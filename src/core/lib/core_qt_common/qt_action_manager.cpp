#include "qt_action_manager.hpp"
#include "core_string_utils/string_utils.hpp"
#include "core_ui_framework/i_action.hpp"

namespace wgt
{
class QtAction : public IAction
{
public:
	static const char pathDelimiter;

	QtAction(QtActionManager& actionManager, 
			 const char* id,
             const char* text, 
             const char* icon, 
             const char* windowId, 
             const char* path,
             const char* shortcut, 
             int order, 
             std::function<void(IAction*)>& func,
	         std::function<bool(const IAction*)>& enableFunc, 
             std::function<bool(const IAction*)>& checkedFunc,
			 std::function<bool(const IAction*)>& visibleFunc,
             const char* group, 
             bool isSeparator = false)
		: actionManager_(actionManager)
		, id_(id)
		, text_(text)
		, icon_(icon)
		, windowId_(windowId)
		, paths_(StringUtils::split(path, pathDelimiter))
		, shortcut_(shortcut)
		, order_(order)
		, func_(func)
		, enableFunc_(enableFunc)
		, checkedFunc_(checkedFunc)
		, visibleFunc_(visibleFunc)
		, checkable_(checkedFunc ? true : false)
		, group_(group)
		, isSeparator_(isSeparator)
		, isVisible_(true)
	{
	}

	~QtAction()
	{
		actionManager_.onQtActionDestroy(this);
	}

	void text(const char* value) override
	{
		if(text_ != value)
		{
			text_ = value;
			signalTextChanged(value);
		}
	}

	const char* text() const override
	{
		return text_.c_str();
	}

	const char* icon() const override
	{
		return icon_.c_str();
	}

	const char* windowId() const override
	{
		return windowId_.c_str();
	}

	const std::vector<std::string>& paths() const override
	{
		return paths_;
	}

	const char* shortcut() const override
	{
		return shortcut_.c_str();
	}

	const char* group() const override
	{
		return group_.c_str();
	}

	const char* id() const override
	{
		return id_.c_str();
	}

	void setShortcut(const char* shortcut) override
	{
		if(shortcut_ != shortcut)
		{
			shortcut_ = shortcut;
			signalShortcutChanged(shortcut);
		}
	}

	int order() const override
	{
		return order_;
	}

	bool enabled() const override
	{
		return enableFunc_(this);
	}

	bool checked() const override
	{
		return checkedFunc_(this);
	}

	bool isCheckable() const override
	{
		return checkable_;
	}

	bool visible() const override
	{
		return isVisible_ && visibleFunc_(this);
	}
	
	void visible(bool visible)
	{
		isVisible_ = visible;
	}

	bool isSeparator() const override
	{
		return isSeparator_;
	}

	void execute() override
	{
		func_(this);
	}

	virtual void setData(const Variant& data) override
	{
		data_ = data;
	}

	Variant& getData() override
	{
		return data_;
	}

	const Variant& getData() const override
	{
		return data_;
	}

private:
	QtActionManager& actionManager_;
	std::string text_;
	std::string icon_;
	std::string windowId_;
	std::string id_;
	std::vector<std::string> paths_;
	std::string shortcut_;
	int order_;
	std::function<void(IAction*)> func_;
	std::function<bool(const IAction*)> enableFunc_;
	std::function<bool(const IAction*)> checkedFunc_;
	std::function<bool(const IAction*)> visibleFunc_;
	Variant data_;
	bool checkable_;
	std::string group_;
	bool isSeparator_;
	bool isVisible_;
};

const char QtAction::pathDelimiter = ';';

QtActionManager::~QtActionManager()
{
}

std::unique_ptr<IAction> QtActionManager::createAction(const char* id, const char* text, const char* icon, const char* windowId,
                                                       const char* path, const char* shortcut, int order,
                                                       std::function<void(IAction*)> func,
                                                       std::function<bool(const IAction*)> enableFunc,
                                                       std::function<bool(const IAction*)> checkedFunc,
													   std::function<bool(const IAction*)> visibleFunc,
                                                       const char* group, bool isSeparator)
{
	return std::unique_ptr<IAction>(new QtAction(*this, id, text, icon, windowId, path, shortcut, order, func, enableFunc,
	                                             checkedFunc, visibleFunc, group, isSeparator));
}

void QtActionManager::onQtActionDestroy(IAction* action)
{
	this->onActionDestroyed(action);
}

} // end namespace wgt
