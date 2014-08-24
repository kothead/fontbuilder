#include "bmfontexporter.h"
#include "../fontconfig.h"
#include "../layoutdata.h"

#include <QString>
BMFontExporter::BMFontExporter(QObject *parent) :
    AbstractExporter(parent)
{
    setExtension("fnt");
}

bool BMFontExporter::Export(QByteArray& out) {
    QString res = "info";
    res += QString(" face=\"") + fontConfig()->family() + QString("\"");
    res += QString(" size=") + QString::number(fontConfig()->size());

    res += QString("\ncommon");
    res += QString(" lineHeight=") + QString::number(metrics().height);
    res += QString(" base=") + QString::number(metrics().height);
    res += QString(" pages=1");

    res += QString("\npage");
    res += QString(" id=0");
    res += QString(" file=") + texFilename();

    res += QString("\nchars");
    res += QString(" count=") + QString::number(symbols().size());
    
    foreach (const Symbol& c , symbols()) {
        res += "\nchar";
        res += QString(" id=") + QString::number(c.id);
        res += QString(" x=") + QString::number(c.placeX);
        res += QString(" y=") + QString::number(c.placeY);
        res += QString(" width=") + QString::number(c.placeW);
        res += QString(" height=") + QString::number(c.placeH);
        res += QString(" xoffset=") + QString::number(c.offsetX);
        res += QString(" yoffset=") + QString::number(metrics().height - c.offsetY);
        res += QString(" xadvance=") + QString::number(c.advance);
        res += QString(" page=0");
        res += QString(" chnl=0");
    }

    QString kernings;
    int kernNumber = 0;
    foreach (const Symbol& c , symbols()) {
        typedef QMap<uint, int>::ConstIterator Kerning;
        for (Kerning k = c.kerning.begin(); k != c.kerning.end(); k++) {
            kernings +="\nkerning";
            kernings += QString(" first=") + QString::number(c.id);
            kernings += QString(" second=") + QString::number(k.key());
            kernings += QString(" amount=") + QString::number(k.value());
            kernNumber++;
        }
    }
    res += QString("\nkernings count=") + QString::number(kernNumber);
    res += kernings;
    out = res.toUtf8();
    return true;
}


AbstractExporter* BMFontExporterFactoryFunc (QObject* parent) {
    return new BMFontExporter(parent);
}
