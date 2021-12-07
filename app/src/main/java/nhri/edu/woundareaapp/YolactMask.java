package nhri.edu.woundareaapp;

import android.graphics.Color;

import java.util.Random;

class YolactMask {
    public float left, top, right, bottom;
    public int label;
    public float prob;
    public float[] maskdata;
    public char[] mask;

    public static String[] labels = {"Background", "Slough", "Granulation", "Scab", "Eschar", "Stasis", "Wound"};
    public static String[] labels_ch = {"背景", "腐肉", "肉芽", "結痂", "焦痂", "瘀傷", "創傷"};

    public int colors[][] = {
            {  0,   0,   0},  //__Background__
            {255,   0,   0},
            {255,   0, 255},
            {  0, 255,   0},
            {255, 255,   0},
            {  0, 255, 255},
            {  0,   0, 255}
    };

    public YolactMask(float left, float top, float right, float bottom, int label, float prob, float[] maskdata, char[] mask) {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
        this.label = label;
        this.prob = prob;
        this.maskdata = maskdata;
        this.mask = mask;
    }

    public int getColor() {
        Random random = new Random(label);
        // return Color.argb(255, random.nextInt(256), random.nextInt(256), random.nextInt(256));
        return Color.argb(255, colors[label % 7][0], colors[label % 7][1], colors[label % 7][2]);
    }

    public float getLeft() {
        return left;
    }

    public void setLeft(float left) {
        this.left = left;
    }

    public float getTop() {
        return top;
    }

    public void setTop(float top) {
        this.top = top;
    }

    public float getRight() {
        return right;
    }

    public void setRight(float right) {
        this.right = right;
    }

    public float getBottom() {
        return bottom;
    }

    public void setBottom(float bottom) {
        this.bottom = bottom;
    }

    public String getLabel() {
        // return labels[label];
        return labels_ch[label];
    }

    public void setLabel(int label) {
        this.label = label;
    }

    public float getProb() {
        return prob;
    }

    public void setProb(float prob) {
        this.prob = prob;
    }

    public float[] getMaskdata() {
        return maskdata;
    }

    public void setMaskdata(float[] maskdata) {
        this.maskdata = maskdata;
    }

    public char[] getMask() {
        return mask;
    }

    public void setMask(char[] mask) {
        this.mask = mask;
    }
}
