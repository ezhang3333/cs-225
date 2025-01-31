#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image& picture)
{
    basePicture = Image(picture);
}

int StickerSheet::addSticker(Image& sticker, int x, int y)
{
    for(unsigned int i = 0; i < StickerVector.size(); i++)
    {
        if(StickerVector[i] == NULL)
        {
            StickerVector[i] = &sticker;
            xCoordinates[i] = x;
            yCoordinates[i] = y;
            return i;
        }
    }

    StickerVector.push_back(&sticker);
    xCoordinates.push_back(x);
    yCoordinates.push_back(y);
    return (StickerVector.size() - 1);
}

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned int layer, int x, int y)
{
    for(unsigned int i = 0; i < StickerVector.size(); i++)
    {
        if(i == layer)
        {
            StickerVector[i] = &sticker;
            xCoordinates[i] = x;
            yCoordinates[i] = y;
            return i;
        }
    }

    StickerVector.push_back(&sticker);
    xCoordinates.push_back(x);
    yCoordinates.push_back(y);
    return (StickerVector.size() - 1);
}

bool StickerSheet::translate(unsigned int index, int x, int y)
{
    if(index >= StickerVector.size() || StickerVector[index] == NULL)
    {
        return false;
    }
    else
    {
        xCoordinates[index] = x;
        yCoordinates[index] = y;
        return true;
    }
}

void StickerSheet::removeSticker(unsigned index)
{
    if(index < StickerVector.size())
    {
        StickerVector[index] = NULL;
        xCoordinates[index] = 0;
        yCoordinates[index] = 0;
    }
}

Image* StickerSheet::getSticker(unsigned index)
{
    if(index >= StickerVector.size() || StickerVector[index] == NULL)
    {
        return NULL;
    }
    
    return StickerVector[index];
}

int StickerSheet::layers() const
{
    return StickerVector.size();
}

Image StickerSheet::render() const
{
    int leftMostCoordinate = 0;
    int topMostCoordinate = 0;

    int baseHeight = basePicture.height();
    int baseWidth = basePicture.width();

    for(unsigned int i = 0; i < StickerVector.size(); i++)
    {
        if(StickerVector[i] == NULL)
        {
            continue;
        }

        int layerImageWidth = xCoordinates[i] + (int)StickerVector[i]->width();
        int layerImageHeight = yCoordinates[i] + (int)StickerVector[i]->height();

        leftMostCoordinate = leftMostCoordinate >= xCoordinates[i] ? xCoordinates[i] : leftMostCoordinate;
        baseWidth = layerImageWidth > baseWidth ? layerImageWidth : baseWidth;
        topMostCoordinate = topMostCoordinate >= yCoordinates[i] ? yCoordinates[i] : topMostCoordinate;
        baseHeight = layerImageHeight > baseHeight ? layerImageHeight : baseHeight;
    }

    Image renderedImage = Image(baseWidth - leftMostCoordinate, baseHeight - topMostCoordinate);

    for(unsigned int i = 0; i < basePicture.height(); i++)
    {
        for(unsigned int m = 0; m < basePicture.width(); m++)
        {
            if(basePicture.getPixel(m,i).a != 0)
            {
                renderedImage.getPixel(m - leftMostCoordinate, i - topMostCoordinate) = basePicture.getPixel(m,i);
            }
        }
    }

    for(unsigned int i = 0; i < StickerVector.size(); i++)
    {
        if(StickerVector[i] == NULL)
        {
            continue;
        }

        for(unsigned int m = 0; m < StickerVector[i]->width(); m++)
        {
            for(unsigned int j = 0; j < StickerVector[i]->height(); j++)
            {
                if(StickerVector[i]->getPixel(m,j).a != 0)
                {
                    renderedImage.getPixel(xCoordinates[i] + m - leftMostCoordinate, yCoordinates[i] + j - topMostCoordinate) = StickerVector[i]->getPixel(m,j);
                }
            }
        }
    }

    return renderedImage;
}