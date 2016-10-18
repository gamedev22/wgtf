
#include "hotloading_panel.hpp"
#include "core_qt_common/helpers/qt_helpers.hpp"
#include <QUrl>

namespace wgt
{
void HotloadingPanel::initialise(QQmlEngine& engine, IFileSystem& filesystem)
{
	fileSystem_ = &filesystem;
	initialiseFile(engine, "WGHotloading.qml", hotLoadedFile_);
	initialiseFile(engine, "WGHotloadingBase.qml", hotLoadedBaseFile_);
	initialiseFile(engine, "wg_hotloading.js", hotLoadedJSFile_);
}

void HotloadingPanel::initialiseFile(QQmlEngine& engine, const char* name, HotloadedFile& file)
{
	const std::string folder(PROJECT_RESOURCE_FOLDER);
	file.path_ = QtHelpers::resolveQmlPath(engine, (folder + name).c_str()).toLocalFile().toUtf8().constData();
	file.text_ = getTextFromFile(file.path_);
}

const HotloadingPanel* HotloadingPanel::getSource() const
{
	return this;
}

std::string HotloadingPanel::getHotloadingText()
{
	return hotLoadedFile_.text_;
}

std::string HotloadingPanel::getHotloadingBaseText()
{
	return hotLoadedBaseFile_.text_;
}

std::string HotloadingPanel::getHotloadingJSText()
{
	return hotLoadedJSFile_.text_;
}

std::string HotloadingPanel::getErrorText()
{
	return errorText_;
}

void HotloadingPanel::setErrorText(const char* text)
{
	errorText_ = text;
}

void HotloadingPanel::setHotloadingText(std::string text)
{
	if (errorText_.empty())
	{
		hotLoadedFile_.text_ = text;
		saveTextToFile(hotLoadedFile_.path_, text);
	}
}

void HotloadingPanel::setHotloadingBaseText(std::string text)
{
	if (errorText_.empty())
	{
		hotLoadedBaseFile_.text_ = text;
		saveTextToFile(hotLoadedBaseFile_.path_, text);
	}
}

void HotloadingPanel::setHotloadingJSText(std::string text)
{
	if (errorText_.empty())
	{
		hotLoadedJSFile_.text_ = text;
		saveTextToFile(hotLoadedJSFile_.path_, text);
	}
}

std::string HotloadingPanel::getTextFromFile(const std::string& path)
{
	if (!fileSystem_->exists(path.c_str()))
	{
		NGT_ERROR_MSG("File %s does not exist", path.c_str());
		return std::string();
	}

	const int bufferSize = 1024;
	char buffer[bufferSize];
	memset(buffer, '\0', bufferSize);

	auto stream = fileSystem_->readFile(path.c_str(), std::ios::in);
	if (!stream)
	{
		NGT_ERROR_MSG("File %s could not be read from", path.c_str());
		return std::string();
	}

	stream->read(buffer, bufferSize);
	return std::string(buffer);
}

void HotloadingPanel::saveTextToFile(const std::string& path, const std::string& text)
{
	if (!fileSystem_->exists(path.c_str()))
	{
		NGT_ERROR_MSG("File %s does not exist", path.c_str());
	}

	if (!fileSystem_->writeFile(path.c_str(), &text[0], text.size(), std::ios::out))
	{
		NGT_ERROR_MSG("File %s could not be written to", path.c_str());
	}
}
} // end namespace wgt