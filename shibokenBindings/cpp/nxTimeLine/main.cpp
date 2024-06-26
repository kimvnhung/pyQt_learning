#include "player.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Player Example");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt MultiMedia Player Example");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL(s) to open.");
    parser.process(app);

    Player player;

    if (!parser.positionalArguments().isEmpty() && player.isPlayerAvailable()) {
        QList<QUrl> urls;
        for (auto &a : parser.positionalArguments())
            urls.append(QUrl::fromUserInput(a, QDir::currentPath()));
        player.addToPlaylist(urls);
    }

    player.show();
    return app.exec();
}
