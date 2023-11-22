#include "SubmarineFree.hpp"

template<typename T>
struct TDS_Module : T {
	using T::T;
	float voltage0 = 0.f;
	float voltage1 = 5.f;
	float midpoint();
	float output(int);
	// DS_Module(int p, int i, int o, int l) : Module(p, i, o, l) {}
	json_t *toJson() override;
	void fromJson(json_t *) override;
	void onReset() override;
	void appendContextMenu(Menu *);
};

typedef TDS_Module<Module> DS_Module;
typedef TDS_Module<ModuleV1> DS_ModuleV1;

template<typename T>
struct TDS_MenuItem : MenuItem {
	TDS_Module<T> *module;
	float vh;
	float vl;
	void onAction(EventAction &e) override;
	void step() override;
};

struct DS_Schmitt {
	int _state = 0;
	static float high(float, float);
	static float low(float, float);
	void reset();
	void set();
	void set(int);
	int state(float, float, float);
	int state(DS_Module *, float);
	int state(DS_ModuleV1 *, float);
	int edge(float, float, float);
	int edge(DS_Module *, float);
	int edge(DS_ModuleV1 *, float);
	int edge(float, float, float, int);
	int edge(DS_Module *, float, int);
	int edge(DS_ModuleV1 *, float, int);
	int redge(float, float, float);
	int redge(DS_Module *, float);
	int redge(DS_ModuleV1 *, float);
	int fedge(float, float, float);
	int fedge(DS_Module *, float);
	int fedge(DS_ModuleV1 *, float);
};
