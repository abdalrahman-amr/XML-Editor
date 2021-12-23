#ifndef MAINBLOCK_H
#define MAINBLOCK_H

#include <QString>
#include <QMap>
#include <QVector>

typedef enum TagType
{
    OPEN_TAG,
    CLOSED_TAG,
    TAG_CONTENT,
    TAG_INFO
} TagType;


class MainBlock
{
protected:
    QString blockContent;
    TagType tagType;
public:
    MainBlock(QString content, TagType type);
    QString getBlockContent();
    void setBlockContent(QString content);
    TagType getBlockTagType();
    void setBlockTagType(TagType type);
    virtual QVector<MainBlock *> * getInternalBlocks() = 0;
    //virtual QMap<QString, QString> * getTagAttributes() = 0;
    virtual ~MainBlock();
};

#endif // MAINBLOCK_H
