
#include "stats.h"
#include <math.h>

stats::stats(PNG & im){
    for (unsigned int i = 0; i < (im.width()); i++) {
        vector<long> temp_r;
        vector<long> temp_g;
        vector<long> temp_b;
        vector<long> temp_rsq;
        vector<long> temp_gsq;
        vector<long> temp_bsq;
        for (unsigned int j=0; j < im.height(); j++) {
            long sumr;
            long sumg;
            long sumb;
            long sumrsq;
            long sumgsq;
            long sumbsq;
            if (i > 0) {
                sumr = sumRed[i-1][j] + im.getPixel(i,j)->r;
                sumg = sumGreen[i-1][j] + im.getPixel(i,j)->g;
                sumb = sumBlue[i-1][j] + im.getPixel(i,j)->b;
                sumrsq = sumsqRed[i-1][j] + pow(im.getPixel(i,j)->r,2);
                sumgsq = sumsqGreen[i-1][j] + pow(im.getPixel(i,j)->g,2);
                sumbsq = sumsqBlue[i-1][j] + pow(im.getPixel(i,j)->b,2);
            } else {
                sumr = im.getPixel(i,j)->r;
                sumg = im.getPixel(i,j)->g;
                sumb = im.getPixel(i,j)->b;
                sumrsq = pow(im.getPixel(i,j)->r,2);
                sumgsq = pow(im.getPixel(i,j)->g,2);
                sumbsq = pow(im.getPixel(i,j)->b,2);
            }
            if (j > 0) {
                for (unsigned int k = 0; k < j; k++) {
                    sumr += im.getPixel(i,k)->r;
                    sumg += im.getPixel(i,k)->g;
                    sumb += im.getPixel(i,k)->b;
                    sumrsq += pow(im.getPixel(i,k)->r,2);
                    sumgsq += pow(im.getPixel(i,k)->g,2);
                    sumbsq += pow(im.getPixel(i,k)->b,2);
                }
            }
            temp_r.push_back(sumr);
            temp_g.push_back(sumg);
            temp_b.push_back(sumb);
            temp_rsq.push_back(sumrsq);
            temp_gsq.push_back(sumgsq);
            temp_bsq.push_back(sumbsq);
        }
        sumRed.push_back(temp_r);
        sumGreen.push_back(temp_g);
        sumBlue.push_back(temp_b);
        sumsqRed.push_back(temp_rsq);
        sumsqGreen.push_back(temp_gsq);
        sumsqBlue.push_back(temp_bsq);
    }
}

long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
    long sum = 0;
    if (ul.first != 0 && ul.second != 0) {
    if (channel == 'r') {
        sum = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1] - sumRed[ul.first - 1][lr.second] 
        + sumRed[ul.first - 1][ul.second - 1];
    } else if (channel == 'g') {
        sum = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1] - sumGreen[ul.first - 1][lr.second] 
        + sumGreen[ul.first - 1][ul.second - 1];
    } else {
        sum = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1] - sumBlue[ul.first - 1][lr.second] 
        + sumBlue[ul.first - 1][ul.second - 1];
    }
    } else if (ul.first != 0 && ul.second == 0) {
        if (channel == 'r') {
            sum = sumRed[lr.first][lr.second] - sumRed[ul.first - 1][lr.second];
        } else if (channel == 'g') {
            sum = sumGreen[lr.first][lr.second] - sumGreen[ul.first - 1][lr.second];
        } else {
            sum = sumBlue[lr.first][lr.second] - sumBlue[ul.first - 1][lr.second];
        }
    } else if (ul.first == 0 && ul.second != 0) {
        if (channel == 'r') {
            sum = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1];
        } else if (channel == 'g') {
            sum = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1];
        } else {
            sum = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1];
        }
    } else {
        if (channel == 'r') {
            sum = sumRed[lr.first][lr.second];
        } else if (channel == 'g') {
            sum = sumGreen[lr.first][lr.second];
        } else {
            sum = sumBlue[lr.first][lr.second];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
    long sumsq = 0;
    if (ul.first != 0 && ul.second != 0) {
    if (channel == 'r') {
        sumsq = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1] - sumsqRed[ul.first - 1][lr.second] 
        + sumsqRed[ul.first - 1][ul.second - 1];
    } else if (channel == 'g') {
        sumsq = sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1] - sumsqGreen[ul.first - 1][lr.second] 
        + sumsqGreen[ul.first - 1][ul.second - 1];
    } else {
        sumsq = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1] - sumsqBlue[ul.first - 1][lr.second] 
        + sumsqBlue[ul.first - 1][ul.second - 1];
    }
    } else if (ul.first != 0 && ul.second == 0) {
        if (channel == 'r') {
            sumsq = sumsqRed[lr.first][lr.second] - sumsqRed[ul.first - 1][lr.second];
        } else if (channel == 'g') {
            sumsq = sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first - 1][lr.second];
        } else {
            sumsq = sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first - 1][lr.second];
        }
    } else if (ul.first == 0 && ul.second != 0) {
        if (channel == 'r') {
            sumsq = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1];
        } else if (channel == 'g') {
            sumsq = sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1];
        } else {
            sumsq = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1];
        }
    } else {
        if (channel == 'r') {
            sumsq = sumsqRed[lr.first][lr.second];
        } else if (channel == 'g') {
            sumsq = sumsqGreen[lr.first][lr.second];
        } else {
            sumsq = sumsqBlue[lr.first][lr.second];
        }
    }
    return sumsq;
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.second - ul.second + 1) * (lr.first - ul.first + 1);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long sumr = getSumSq('r',ul,lr) - pow(getSum('r',ul,lr),2)/rectArea(ul,lr);
    long sumg = getSumSq('g',ul,lr) - pow(getSum('g',ul,lr),2)/rectArea(ul,lr);
    long sumb = getSumSq('b',ul,lr) - pow(getSum('b',ul,lr),2)/rectArea(ul,lr);
    return sumr + sumg + sumb;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    long area = rectArea(ul,lr);
    int r = getSum('r',ul,lr)/area;
    int g = getSum('g',ul,lr)/area;
    int b = getSum('b',ul,lr)/area;
    RGBAPixel* pixel = new RGBAPixel(r,g,b);
    return *pixel;
}
