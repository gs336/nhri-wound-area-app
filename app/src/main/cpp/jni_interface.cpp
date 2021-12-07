#include <jni.h>
#include <string>
#include <ncnn/gpu.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include "Yolact.h"
#include "ocr.h"


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    ncnn::create_gpu_instance();
    if (ncnn::get_gpu_count() > 0) {
        Yolact::hasGPU = true;
        OCR::hasGPU = true;
    }
//    LOGD("jni onload");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    ncnn::destroy_gpu_instance();
    delete Yolact::detector;
    delete OCR::detector;
//    LOGD("jni onunload");
}

/*********************************************************************************************
                                         Yolact
 ********************************************************************************************/
jintArray matToBitmapIntArray(JNIEnv *env, const cv::Mat &image) {
    jintArray resultImage = env->NewIntArray(image.total());
    auto *_data = new jint[image.total()];
    for (int i = 0; i < image.total(); i++) {  // =========== 注意这里再确认下要不要除3
        char r = image.data[3 * i + 2];
        char g = image.data[3 * i + 1];
        char b = image.data[3 * i + 0];
        char a = (char) 255;
        _data[i] = (((jint) a << 24) & 0xFF000000) + (((jint) r << 16) & 0x00FF0000) +
                   (((jint) g << 8) & 0x0000FF00) + ((jint) b & 0x000000FF);
    }
    env->SetIntArrayRegion(resultImage, 0, image.total(), _data);
    delete[] _data;
    return resultImage;
}

jcharArray matToBitmapCharArray(JNIEnv *env, const cv::Mat &image) {
    jcharArray resultImage = env->NewCharArray(image.total());
    auto *_data = new jchar[image.total()];
    for (int i = 0; i < image.total(); i++) {
        char m = image.data[i];
        _data[i] = (m & 0xFF);
    }
    env->SetCharArrayRegion(resultImage, 0, image.total(), _data);
    delete[] _data;
    return resultImage;
}

extern "C" JNIEXPORT void JNICALL
Java_nhri_edu_woundareaapp_Yolact_init(JNIEnv *env, jclass clazz, jobject assetManager, jboolean useGPU) {
    if (Yolact::detector != nullptr) {
        delete Yolact::detector;
        Yolact::detector = nullptr;
    }
    if (Yolact::detector == nullptr) {
        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        Yolact::detector = new Yolact(mgr, useGPU);
    }
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_nhri_edu_woundareaapp_Yolact_detect(JNIEnv *env, jclass clazz, jobject image) {
    auto result = Yolact::detector->detect_yolact(env, image);

    auto yolact_mask = env->FindClass("nhri/edu/woundareaapp/YolactMask");
//    auto cid = env->GetMethodID(yolact_mask, "<init>", "(FFFFIF[F[I)V");
    auto cid = env->GetMethodID(yolact_mask, "<init>", "(FFFFIF[F[C)V");
    jobjectArray ret = env->NewObjectArray(result.size(), yolact_mask, nullptr);
    int i = 0;
    for (auto &mask : result) {
//        LOGD("jni yolact mask rect x:%f y:%f", mask.rect.x, mask.rect.y);
//        LOGD("jni yolact maskdata size:%d", mask.maskdata.size());
//        LOGD("jni yolact mask size:%d", mask.mask.cols * mask.mask.rows);
//        jintArray jintmask = matToBitmapIntArray(env, mask.mask);
        jcharArray jcharmask = matToBitmapCharArray(env, mask.mask);

        env->PushLocalFrame(1);
        jfloatArray maskdata = env->NewFloatArray(mask.maskdata.size());
        auto *jnum = new jfloat[mask.maskdata.size()];
        for (int j = 0; j < mask.maskdata.size(); ++j) {
            *(jnum + j) = mask.maskdata[j];
        }
        env->SetFloatArrayRegion(maskdata, 0, mask.maskdata.size(), jnum);
        delete[] jnum;

        jobject obj = env->NewObject(yolact_mask, cid,
                                     mask.rect.x, mask.rect.y, mask.rect.x + mask.rect.width,
                                     mask.rect.y + mask.rect.height,
                                     mask.label, mask.prob, maskdata, jcharmask);
        obj = env->PopLocalFrame(obj);
        env->SetObjectArrayElement(ret, i++, obj);
    }
    return ret;
}


/*********************************************************************************************
                                         chineseocr-lite
 ********************************************************************************************/
jstring str2jstring(JNIEnv *env, const char *pat) {
    //定义java String类 strClass
    jclass strClass = (env)->FindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("UTF-8");
    //将byte数组转换为java String,并输出
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

std::string jstring2str(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    std::string stemp(rtn);
    free(rtn);
    return stemp;
}