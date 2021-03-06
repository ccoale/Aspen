#include <angelscript.h>
#include <scriptstdstring.h>
#include <scriptarray.h>
#include <sstream>
#include <cassert>
#include "../mud.h"
#include "../conf.h"
#include "../world.h"
#include "../event.h"
#include "script.h"
#include "scr_BaseObject.h"
#include "scr_Entity.h"


static void MessageCallback(const asSMessageInfo *msg, void *param)
{
    World* world = World::GetPtr();
    std::stringstream st;

//error type:
    switch(msg->type)
        {
        case asMSGTYPE_ERROR:
            st << "[ERROR]";
            break;
        case asMSGTYPE_WARNING:
            st << "[WARNING]";
            break;
        case asMSGTYPE_INFORMATION:
            st << "[INFORMATION]";
            break;
        default:
            st << "[UNKNOWN]";
        }

    st << " @" << msg->section << ":";
    st << "LN " << msg->row << ", COL " << msg->col << ":";
    st << msg->message;
    world->WriteLog(st.str(), SCRIPT);
}

ScriptEngine* ScriptEngine::_ptr;
ScriptEngine::ScriptEngine()
{
    _engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    RegisterScriptArray(_engine, true);
    RegisterStdString(_engine);
    RegisterStdStringUtils(_engine);
    _engine->SetMessageCallback(asFUNCTION(MessageCallback), NULL, asCALL_CDECL);
}
ScriptEngine::~ScriptEngine()
{
    _engine->Release();
}

void ScriptEngine::Initialize()
{
    _ptr = new ScriptEngine();
}
void ScriptEngine::Release()
{
    if (_ptr)
        {
            delete _ptr;
            _ptr = nullptr;
        }
}
ScriptEngine* ScriptEngine::GetPtr()
{
    return _ptr;
}

asIScriptEngine* ScriptEngine::GetBaseEngine()
{
    return _engine;
}
bool ScriptEngine::RegisterMethod(const char* obj, const char* decl, asSFuncPtr ptr)
{
    int ret = 0;
    ret = _engine->RegisterObjectMethod(obj, decl, ptr, asCALL_THISCALL);
    return (ret >= 0? true : false);
}
bool ScriptEngine::RegisterMethod(const char* obj, const char* decl, asSFuncPtr ptr, asDWORD callConv)
{
    int ret = 0;
    ret = _engine->RegisterObjectMethod(obj, decl, ptr, callConv);
    return (ret >= 0? true : false);
}
bool ScriptEngine::RegisterObject(const char* obj)
{
    int r = 0;

    r = _engine->RegisterObjectType(obj, 0, asOBJ_REF|asOBJ_NOCOUNT);
    return (r >= 0? true : false);
}

CEVENT(ScriptEngine, Shutdown)
{
    World* world = World::GetPtr();
    world->WriteLog("Cleaning up scripting.");
    ScriptEngine::Release();
}

/**
we initialize individual objects here.
This needs to be done separate from the methods,
as methods sometimes rely on other objects.
*/
static void InitializeObjects()
{
    bool ret = false;
    ScriptEngine* engine = ScriptEngine::GetPtr();

    ret = engine->RegisterObject("BaseObject");
    assert(ret);
    ret = engine->RegisterObject("Entity");
    assert(ret);
}
/**
Initializes individual methods, properties and etc on specific objects.
*/
static void InitializeObjectTraits()
{
    InitializeBaseObject();
    InitializeEntity();
}

bool InitializeScript()
{
    World* world = World::GetPtr();
    ScriptEngine* engine = nullptr;

    world->WriteLog("Initializing scripting.");

    ScriptEngine::Initialize();
    engine = ScriptEngine::GetPtr();
    world->events.AddCallback("Shutdown", std::bind(&ScriptEngine::Shutdown, engine, std::placeholders::_1, std::placeholders::_2));

    InitializeObjects();
    InitializeObjectTraits();

    return true;
}
