#include "StdAfx.h"
#include "Keywords.h"
#include "utils.h"
#include "xmlParser.h"

CVarValue ParseNode(XMLNode node)
{
    CVarValue val;
    int atrnum = node.nAttribute();
    if(atrnum)
    {
        val.set_hash(); 
        for(int i=0;i<atrnum;i++)
        {
            val.set_hash_value(CVarValue(node.getAttributeName(i)),
                               CVarValue(node.getAttributeValue(i)));
        }
    }
    int nodenum = node.nChildNode();
    if(nodenum)
    {
        if(val.type() != VT_HASH)val.set_hash();

        CVarValueMap&  vmap = const_cast<CVarValueMap&>(val.get_hash());
        for(int i=0;i<nodenum;i++)
        {       
            XMLNode child = node.getChildNode(i);
            CVarValue name(child.getName());
            CVarValueMap::iterator it = vmap.find(name);
            if(it == vmap.end())
            {
                vmap[name] = ParseNode(child);
            }
            else
            {
                it->second.append_value(ParseNode(child));
            }
        }
    }
    return val;
}


struct CXMLparseKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt){

    wstring xml = param[0].to_string();
    wstring tag = param[1].to_string();

    XMLResults Results;
    XMLNode root =  XMLNode::parseString(xml.c_str(),tag.c_str(),&Results);
    if(Results.error != eXMLErrorNone)
    {
        return CVarValue(); // xml not parsed
    }
    return ParseNode(root);
}};static KeywordAutoReg<CXMLparseKW> sXMLparseKW(L"parsedcxml");

struct CMakeXActionsKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapActionProcessor* Actions = 
        GetDCMapCore()->GetServiceTyped<IDCMapActionProcessor>("ActionProcessor");
    if(!Actions)return CVarValue();

    wstring player = param[0].to_string();
    dcmapDCSTR xml = Actions->MakeRequest(player.c_str(),0,0);

    if(!xml || !xml[0])return CVarValue();

    return CVarValue(xml);
}};
static KeywordAutoReg<CMakeXActionsKW> sCMakeXActionsKW(L"make_xactions");

struct CProcessXActionsKW: CBaseKeyword{CVarValue Execute(CVarValue const& param,CBasicBlockParser* cnt)
{
    IDCMapActionProcessor* Actions = 
        GetDCMapCore()->GetServiceTyped<IDCMapActionProcessor>("ActionProcessor");
    if(!Actions)return CVarValue();

    wstring player = param[0].to_string();

    bool r = Actions->ProcessResponse(player.c_str(),0,param[1].as_string().c_str());

    return CVarValue(r);
}};
static KeywordAutoReg<CProcessXActionsKW> CProcessXActionsKW(L"process_xactions_response");
