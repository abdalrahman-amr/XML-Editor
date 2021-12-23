#include "xml-tree.h"

XMLTree::XMLTree(QString fileText)
    : fileText(fileText)
    , JSONFile("")
    , indentCounter(0)

{
    analyzeXMLFileText();
}

XMLTree::XMLTree(QFile *file)
    : fileText("")
    , JSONFile("")
    , indentCounter(0)
    , file(file)
{
    convertingXMLFileIntoText();
    analyzeXMLFileText();
}

XMLTree::~XMLTree()
{
    if(xmlRoot != nullptr)
    {
        delete xmlRoot;
    }
}

void XMLTree::convertingXMLFileIntoText()
{
    file->open(QIODevice::ReadOnly);
    QTextStream fileInput(file);

    //Storing each line in the XML file in the QString private member.
    while(!fileInput.atEnd())
    {
        fileText.append(fileInput.readLine());
    }
    file->close();
}

void XMLTree::analyzeXMLFileText()
{
    QVector<QString> *separateTag = breakingFileTextIntoTags();
    buildingXMLTree(separateTag);
}

QVector<QString> * XMLTree::breakingFileTextIntoTags()
{
    QVector<QString> *separateTags = new QVector<QString> ();
    QString currentTag = "";
    QString currentCharacter;
    bool isComment = false;
    bool isLetter = false;

    //TO DO: FIX (int i) an overflow may happen if number of characters exceeds MAX_INT.
    for(int i = 0; i < fileText.length(); i++)
    {
        currentCharacter = fileText[i];

        if(currentCharacter == '<')
        {
            if(fileText[i+1] == '!') {
                isComment = true;
            }
            else if (!isComment){
                //In case of open tag, the current tag is always an empty string.
                //In case of text tag, the current tag has a value so it is pushed in the vector of strings.
                if(!currentTag.isEmpty()) {
                    separateTags->push_back(currentTag);
                    currentTag.clear();
                }
                currentTag += currentCharacter;
                isLetter = true;
            }
        }
        //Indicates the tag end, therefore the current tag is pushed in the vector of strings.
        else if(currentCharacter == '>' && !isComment)
        {
            currentTag += currentCharacter;
            separateTags->push_back(currentTag);
            currentTag.clear();
            isLetter = false;
        }
        else if(isLetter || (currentCharacter != ' ' && currentCharacter != '\t' && currentCharacter != '\n'))
        {
            if(currentCharacter == '-' && isComment) {
                //Indicates the end of a comment
                if(fileText[i+1] == '-' && fileText[i+2] == '>') {
                    isComment = false;
                    i+=2;
                }
            }
            else if(!isComment) {
                currentTag += currentCharacter;
                isLetter = true;
            }
        }
    }

    return separateTags;
}
