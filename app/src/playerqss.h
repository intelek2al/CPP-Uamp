//
// Created by Mykyta Savytskyi on 11/10/20.
//

#ifndef UAMP_PLAYERQSS_H
#define UAMP_PLAYERQSS_H

#include <QString>

inline QString PlayStyle()
{
    return "QPushButton"
           " {"
           "background-image:url(:/image/image/image/play.png);"
           "   background-repeat:no-repeat;"
           "  background-position:center center;"
           "   border:none;"

           "    }"

           "QPushButton:hover{"
           "  background-repeat:no-repeat;"
           "   background-position:center center;"
           "background-image:url(:/image/image/image-hover/play-hover.png);"

           " }"
           " QPushButton:pressed{"
           "background-repeat:no-repeat;"
           " background-position:center center;"
           " background-image:url(:/image/image/image/play.png);"

           "}";
}
inline QString PauseStyle()
{
    return "QPushButton"
           " {"
           "background-image:url(:/image/image/image/pase.png);"
           "   background-repeat:no-repeat;"
           "  background-position:center center;"
           "   border:none;"

           "    }"

           "QPushButton:hover{"
           "  background-repeat:no-repeat;"
           "   background-position:center center;"
           "background-image:url(:/image/image/image-hover/pase-hover.png);"

           " }"
           " QPushButton:pressed{"
           "background-repeat:no-repeat;"
           " background-position:center center;"
           " background-image:url(:/image/image/image/pase.png);"

           "}";
}

inline QString RandomStyle()
{

    return "QPushButton"
           " {"
           "background-image:url(:/image/image/image/play-random.png);"
           "   background-repeat:no-repeat;"
           "  background-position:center center;"
           "   border:none;"

           "    }"

           "QPushButton:hover{"
           "  background-repeat:no-repeat;"
           "   background-position:center center;"
           "background-image:url(:/image/image/image-hover/play-random-hover.png);"

           " }"
           " QPushButton:pressed{"
           "background-repeat:no-repeat;"
           " background-position:center center;"
           " background-image:url(:/image/image/image/play-random.png);"

           "}";

}
inline QString LoopStyle()
{
    return "QPushButton"
           " {"
           "background-image:url(:/image/image/image/loop.png);"
           "   background-repeat:no-repeat;"
           "  background-position:center center;"
           "   border:none;"

           "    }"

           "QPushButton:hover{"
           "  background-repeat:no-repeat;"
           "   background-position:center center;"
           "background-image:url(:/image/image/image-hover/loop-hover.png);"

           " }"
           " QPushButton:pressed{"
           "background-repeat:no-repeat;"
           " background-position:center center;"
           " background-image:url(:/image/image/image/loop.png);"

           "}";

}
inline QString LoopOneStyle()
{
    return "QPushButton"
           " {"
           "background-image:url(:/image/image/image/loop-one.png);"
           "   background-repeat:no-repeat;"
           "  background-position:center center;"
           "   border:none;"

           "    }"

           "QPushButton:hover{"
           "  background-repeat:no-repeat;"
           "   background-position:center center;"
           "background-image:url(:/image/image/image-hover/loop-one-hover.png);"

           " }"
           " QPushButton:pressed{"
           "background-repeat:no-repeat;"
           " background-position:center center;"
           " background-image:url(:/image/image/image/loop-one.png);"

           "}";

}

#endif //UAMP_PLAYERQSS_H
