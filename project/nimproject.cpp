#include "nimproject.h"
#include "nimprojectnode.h"
#include "nimprojectmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QThread>

#include <texteditor/textdocument.h>

namespace NimEditor {

const int MIN_TIME_BETWEEN_PROJECT_SCANS = 4500;

NimProject::NimProject(NimProjectManager *projectManager, const QString &fileName)
    : m_projectManager(projectManager)
    , m_document(new TextEditor::TextDocument)
{
    m_document->setFilePath(Utils::FileName::fromString(fileName));
    m_projectDir = QFileInfo(fileName).dir();
    m_rootNode = new ProjectNode(Utils::FileName::fromString(m_projectDir.dirName()));

    m_projectScanTimer.setSingleShot(true);
    connect(&m_projectScanTimer, SIGNAL(timeout()), this, SLOT(populateProject()));

    populateProject();

    connect(&m_fsWatcher, &QFileSystemWatcher::directoryChanged, this, &NimProject::scheduleProjectScan);
}


QString NimProject::displayName() const
{
    return m_projectDir.dirName();
}

Core::IDocument *NimProject::document() const
{
    return m_document;
}

ProjectExplorer::IProjectManager *NimProject::projectManager() const
{
    return m_projectManager;
}

ProjectExplorer::ProjectNode *NimProject::rootProjectNode() const
{
    return m_rootNode;
}

QStringList NimProject::files(FilesMode) const
{
    return QStringList(m_files.toList());
}

void NimProject::scheduleProjectScan()
{
    auto elapsedTime = m_lastProjectScan.elapsed();
    if (elapsedTime < MIN_TIME_BETWEEN_PROJECT_SCANS) {
        if (!m_projectScanTimer.isActive()) {
            m_projectScanTimer.setInterval(MIN_TIME_BETWEEN_PROJECT_SCANS - elapsedTime);
            m_projectScanTimer.start();
        }
    } else {
        populateProject();
    }
}

void NimProject::populateProject()
{
    m_lastProjectScan.start();

    QSet<QString> oldFiles(m_files);
    m_files.clear();

    recursiveScanDirectory(m_projectDir, m_files);

    auto removedFiles = oldFiles - m_files;
    auto addedFiles = m_files - oldFiles;

    removeNodes(removedFiles);
    addNodes(addedFiles);

    if (removedFiles.size() || addedFiles.size())
        emit fileListChanged();
}

void NimProject::recursiveScanDirectory(const QDir &dir, QSet<QString> &container)
{
    static const QRegExp projectFilePattern(QLatin1String(".*\\.nimproject(?:\\.user)?$"));
    for (const QFileInfo &info : dir.entryInfoList(QDir::AllDirs |
                                                   QDir::Files |
                                                   QDir::NoDotAndDotDot |
                                                   QDir::NoSymLinks |
                                                   QDir::CaseSensitive))
    {
        if (info.isDir())
            recursiveScanDirectory(QDir(info.filePath()), container);
        else if (projectFilePattern.indexIn(info.fileName()) == -1)
            container << info.filePath();
    }
    m_fsWatcher.addPath(dir.absolutePath());
}

void NimProject::addNodes(const QSet<QString> &nodes)
{
    using namespace ProjectExplorer;

    QStringList path;
    for (const QString& node : nodes) {
        path = m_projectDir.relativeFilePath(node).split(QDir::separator());
        path.pop_back();
        FolderNode *folder = findFolderFor(path);
        folder->addFileNodes(QList<FileNode*>() << new FileNode(Utils::FileName::fromString(node),
                                                                SourceType, false));
    }
}

void NimProject::removeNodes(const QSet<QString> &nodes)
{
    using namespace ProjectExplorer;

    QStringList path;
    for (const QString& node : nodes) {
        path = m_projectDir.relativeFilePath(node).split(QDir::separator());
        path.pop_back();
        FolderNode *folder = findFolderFor(path);

        for (FileNode *file : folder->fileNodes()) {
            if (file->path().toString() == node) {
                folder->removeFileNodes(QList<FileNode*>() << file);
                break;
            }
        }
    }
}

ProjectExplorer::FolderNode *NimProject::findFolderFor(const QStringList &path)
{
    using namespace ProjectExplorer;
    FolderNode *folder = m_rootNode;

    for (const QString& part : path) {
        bool folderFound = false;
        foreach (FolderNode *subFolder, folder->subFolderNodes()) {
            if (subFolder->displayName() == part) {
                folder = subFolder;
                folderFound = true;
                break;
            }
        }
        if (!folderFound) {
            FolderNode *newFolder = new FolderNode(Utils::FileName::fromString(part));
            folder->addFolderNodes(QList<FolderNode*>() << newFolder);
            folder = newFolder;
        }
    }
    return folder;
}

}