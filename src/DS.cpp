#include "DS.hpp"

template<typename T>
float TDS_Module<T>::midpoint() {
	return (voltage0 * 0.5f + voltage1 * 0.5f);
}

template<typename T>
json_t *TDS_Module<T>::toJson() {
	json_t *rootJ = json_object();
	json_object_set_new(rootJ, "voltage0", json_real(voltage0));
	json_object_set_new(rootJ, "voltage1", json_real(voltage1));
	return rootJ;
}

template<typename T>
void TDS_Module<T>::fromJson(json_t *rootJ) {
	json_t *j0 = json_object_get(rootJ, "voltage0");
	if (j0)
		voltage0 = json_number_value(j0);
	json_t *j1 = json_object_get(rootJ, "voltage1");
	if (j1)
		voltage1 = json_number_value(j1);
}

template<typename T>
void TDS_Module<T>::onReset() {
	voltage0 = 0.0f;
	voltage1 = 5.0f;
}

template<typename T>
float TDS_Module<T>::output(int state) {
	return state?voltage1:voltage0;
} 

template<typename T>
struct TDS_ParentMenuItem : MenuItem {
	TDS_Module<T> *module;
	Menu *createChildMenu() override {
		Menu *menu = new Menu();
		TDS_MenuItem<T> *m = MenuItem::create<TDS_MenuItem<T>>("0V - 1V");
		m->module = module;
		m->vl = 0.0f;
		m->vh = 1.0f;
		menu->addChild(m);
		m = MenuItem::create<TDS_MenuItem<T>>("0V - 5V");
		m->module = module;
		m->vl = 0.0f;
		m->vh = 5.0f;
		menu->addChild(m);
		m = MenuItem::create<TDS_MenuItem<T>>("0V - 10V");
		m->module = module;
		m->vl = 0.0f;
		m->vh = 10.0f;
		menu->addChild(m);
		m = MenuItem::create<TDS_MenuItem<T>>("-5V - 5V");
		m->module = module;
		m->vl = -5.0f;
		m->vh = 5.0f;
		menu->addChild(m);
		m = MenuItem::create<TDS_MenuItem<T>>("-10V - 10V");
		m->module = module;
		m->vl = -10.0f;
		m->vh = 10.0f;
		menu->addChild(m);
		return menu;
	}
};

template<typename T>
void TDS_Module<T>::appendContextMenu(Menu *menu) {
	menu->addChild(new MenuSeparator());
	TDS_ParentMenuItem<T> *m = MenuItem::create<TDS_ParentMenuItem<T>>("Digital Voltage Range");
	m->module = this;
	m->rightText = SUBMENU;
	menu->addChild(m);
}

template<typename T>
void TDS_MenuItem<T>::onAction(EventAction &e) {
	module->voltage0 = vl;
	module->voltage1 = vh;
}

template<typename T>
void TDS_MenuItem<T>::step() {
	rightText = CHECKMARK((module->voltage0 == vl) && (module->voltage1 == vh));
	MenuItem::step();
}

float DS_Schmitt::high(float v0, float v1) {
	return (v0 * 0.4f + v1 * 0.6f);
}

float DS_Schmitt::low(float v0, float v1) {
	return (v0 * 0.6f + v1 * 0.4f);
}

void DS_Schmitt::reset() {
	_state = 0;
}

void DS_Schmitt::set() {
	_state = 1;
}

void DS_Schmitt::set(int state) {
	_state = state;
}

int DS_Schmitt::state(float vl, float vh, float v) {
	if (_state) {
		if (v < vl)
			_state = 0;
	}
	else {
		if (v > vh)
			_state = 1;
	}
	return _state;
}

int DS_Schmitt::state(DS_Module *module, float v) {
	return state(low(module->voltage0, module->voltage1), high(module->voltage0, module->voltage1), v);
}
int DS_Schmitt::state(DS_ModuleV1 *module, float v) {
	return state(low(module->voltage0, module->voltage1), high(module->voltage0, module->voltage1), v);
}

int DS_Schmitt::edge(float vl, float vh, float v) {
	int old = _state;
	return (state(vl, vh, v) != old);
}
	
int DS_Schmitt::edge(DS_Module *module, float v) {
	int old = _state;
	return (state(module, v) != old);
}
int DS_Schmitt::edge(DS_ModuleV1 *module, float v) {
	int old = _state;
	return (state(module, v) != old);
}

int DS_Schmitt::edge(float vl, float vh, float v, int falling) {
	return falling?fedge(vl, vh, v):redge(vl, vh, v);
}

int DS_Schmitt::edge(DS_Module *module, float v, int falling) {
	return falling?fedge(module, v):redge(module, v);
}
int DS_Schmitt::edge(DS_ModuleV1 *module, float v, int falling) {
	return falling?fedge(module, v):redge(module, v);
}

int DS_Schmitt::redge(float vl, float vh, float v) {
	int old = _state;
	return (state(vl, vh, v) && !old);
}

int DS_Schmitt::redge(DS_Module *module, float v) {
	int old = _state;
	return (state(module, v) && !old);
}
int DS_Schmitt::redge(DS_ModuleV1 *module, float v) {
	int old = _state;
	return (state(module, v) && !old);
}

int DS_Schmitt::fedge(float vl, float vh, float v) {
	int old = _state;
	return (!state(vl, vh, v) && old);
}

int DS_Schmitt::fedge(DS_Module *module, float v) {
	int old = _state;
	return (!state(module, v) && old);
}
int DS_Schmitt::fedge(DS_ModuleV1 *module, float v) {
	int old = _state;
	return (!state(module, v) && old);
}

template struct TDS_Module<Module>;
template struct TDS_Module<ModuleV1>;
