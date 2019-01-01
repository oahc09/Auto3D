#pragma once
#include "Singleton.h"
#include "Auto.h"
#include "Engine.h"
#include "Ambient.h"
namespace Auto3D {

enum class AppStates
{
	Initing,
	Running,
	Pauseing,
	Stopping,
	Exit,
	ErrorExit,
};

class Application
{
public:
	Application();
	~Application();
	/**
	* @brief : Init application
	*/
	bool Init();
	/**
	* @brief : Run frame and update logic
	*/
	bool RunLoop();
	/**
	* @brief : Finish exit application
	*/
	bool Finish();
	/**
	* @brief : Error exit application
	*/
	void ErrorExit();
	/*
	* @brief : this is Engine important funcation init awake runloop and finish run
	*/
	bool Run(SharedPtr<Ambient> ambient);
	/**
	* @brief : Return application states for AppStates type
	*/
	const AppStates GetStates() const { return _appStates; }

private:
	/**
	* @brief : Set states fot application
	*/
	void setStates(AppStates states) { _appStates = states; }
private:
	SharedPtr<Engine> _engine;
	AppStates _appStates;
};



}

