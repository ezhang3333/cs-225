/** 
 * @file Image.cpp
 * Implementation of Image class
 */

#include "Image.h"
#include <cmath>


void Image::lighten()
{
    lighten(0.1);
}


void Image::lighten(double amount)
{
    for(unsigned int i = 0; i < width(); i++)
    {
        for(unsigned int m = 0; m < height(); m++)
        {
            cs225::HSLAPixel & currentPixel = getPixel(i, m);

           currentPixel.l += amount;

            if(currentPixel.l < 0.0)
            {
                currentPixel.l = 0.0;
            }
            else if(currentPixel.l > 1.0)
            {
                currentPixel.l = 1.0;
            }
        }
    }
}

void Image::darken()
{
    lighten(-0.1);
}

void Image::darken(double amount)
{
    lighten(-amount);
}

void Image::saturate()
{
    saturate(0.1);
}

void Image::saturate(double amount)
{
    for(unsigned int i = 0; i < width(); i++)
    {
        for(unsigned int m = 0; m < height(); m++)
        {
            cs225::HSLAPixel & currentPixel = getPixel(i, m);

            currentPixel.s += amount;

            if(currentPixel.s < 0.0)
            {
                currentPixel.s = 0.0;
            }
            else if(currentPixel.s > 1.0)
            {
                currentPixel.s = 1.0;
            }
        }
    }
}

void Image::desaturate()
{
    saturate(-0.1);
}

void Image::desaturate(double amount)
{
    saturate(-amount);
}


void Image::grayscale()
{
    for(unsigned int i = 0; i < width(); i++)
    {
        for(unsigned int m = 0; m < height(); m++)
        {
            cs225::HSLAPixel & currentPixel = getPixel(i,m);

            currentPixel.s = 0.0;
        }
    }
}

void Image::rotateColor(double degrees)
{
    for(unsigned int i = 0; i < width(); i++)
    {
        for(unsigned int m = 0; m < height(); m++)
        {
            cs225::HSLAPixel & currentPixel = getPixel(i,m);

            currentPixel.h += degrees;

            while(currentPixel.h < 0.0)
            {
                currentPixel.h += 360.0;
            }

            currentPixel.h = fmod(currentPixel.h, 360.0);
        }
    }
}

void Image::illinify()
{
    double orangeDifference;
    double blueDifference; 

    double illiniOrange = 11.0;
    double illiniBlue = 216.0;

    for(unsigned int i = 0; i < width(); i++)
    {
        for(unsigned int m = 0; m < height(); m++)
        {
            cs225::HSLAPixel & currentPixel = getPixel(i,m);

            orangeDifference = fabs(currentPixel.h - illiniOrange);
            blueDifference = fabs(currentPixel.h - illiniBlue);

            if(orangeDifference > blueDifference)
            {
                currentPixel.h = illiniBlue;
            }
            else
            {
                currentPixel.h = illiniOrange;
            }
        }
    }
}

void Image::scale(double factor)
{
    int newWidth = width() * factor;
    int newHeight = height() * factor;

    Image scaledImage;
    scaledImage.resize(newWidth, newHeight);

    int originalPixelX;
    int originalPixelY;

    for(int i = 0; i < newWidth; i++)
    {
        for(int m = 0; m < newHeight; m++)
        {
            originalPixelX = i/factor;
            originalPixelY = m/factor;

            scaledImage.getPixel(i,m) = this->getPixel(originalPixelX, originalPixelY);
        }
    }

    this->resize(newWidth, newHeight);

    for(int i = 0; i < newWidth; i++)
    {
        for(int m = 0; m < newHeight; m++)
        {
            this->getPixel(i,m) = scaledImage.getPixel(i,m);
        }
    }
}

void Image::scale(unsigned w, unsigned h)
{
    double widthFactor = w / width();
    double heightFactor = h / height();

    if(widthFactor > heightFactor)
    {
        scale(heightFactor);
    }
    else
    {
        scale(widthFactor);
    }
}