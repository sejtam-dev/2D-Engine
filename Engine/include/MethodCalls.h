#pragma once

class Calls {
public:
	virtual void OnEnabled() = 0;
	virtual void OnDisabled() = 0;

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

class ObjectCalls : public Calls {
	
};

class SceneCalls : public Calls {
	
};

class ComponentCalls : public Calls {

};