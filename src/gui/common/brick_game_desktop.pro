# CONFIG += sanitizer sanitize_address

QT += widgets

INCLUDEPATH  += ../../brick_game/common/ \
        ../../brick_game/tetris/ \
        ../../brick_game/snake/ \
        ./ \
        ../desktop/

SOURCES += *.cc \
        ../../brick_game/snake/*.cc \
        ../../brick_game/tetris/*.cc \
        ../desktop/*.cc