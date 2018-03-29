#include <gvr/utils/AbsJson.h>
#include <cjson/cJSON.h>

#include <cstring>
#include <cstdlib>

extern "C" void (*cJSON_free)(void *ptr);
extern "C" char* cJSON_strdup(const char* str);

namespace gvr
{
  namespace utils
  {
    static char const * CEmptyString = "";
    static char const * CTrueString = "true";

    AbsJson::~AbsJson (void)
    {
      m_pNode = 0;
    }

    AbsJson::AbsJson (AbsJson const & jr)
     : m_pNode(jr.m_pNode)
    {
    }

    char const * AbsJson::name (void) const
    {
      char const * result = CEmptyString;
      if (m_pNode)
      {
        if (m_pNode->string)
        {
          result = m_pNode->string;
        }
      }
      return result;
    }

    AbsJson const AbsJson::get (char const * rhs) const
    {
      AbsJson jr;
      if (rhs)
      {
        if (m_pNode)
        {
          cJSON * pObject = cJSON_GetObjectItem(m_pNode,rhs);
          if (pObject)
          {
            jr.setNode(pObject);
          }
        }
      }
      return jr;
    }

    AbsJson const AbsJson::get (std::string const & rhs) const
    {
      return get(rhs.c_str());
    }

    AbsJson       AbsJson::get (char const * rhs)
    {
      AbsJson jr;
      if (rhs)
      {
        if (m_pNode)
        {
          cJSON * pObject = cJSON_GetObjectItem(m_pNode,rhs);
          if (pObject)
          {
            jr.setNode(pObject);
          }
        }
      }
      return jr;
    }

    AbsJson       AbsJson::get (std::string const & rhs)
    {
      return get(rhs.c_str());
    }

    AbsJson       AbsJson::set (char const * rhs)
    {
      AbsJson jr;
      if (rhs)
      {
        if (m_pNode)
        {
          cJSON * pObject = cJSON_GetObjectItem(m_pNode,rhs);
          if (!pObject)
          {
            pObject = cJSON_CreateNull();
            if (pObject)
            {
              if (m_pNode->type != cJSON_Object)
              {
                private_clearNode(m_pNode);
                m_pNode->type = cJSON_Object;
              }
              cJSON_AddItemToObject(m_pNode,rhs,pObject);
              cJSON * pObjectAux = cJSON_GetObjectItem(m_pNode,rhs);
              if (!pObjectAux)
              {
                cJSON_Delete(pObject);
                pObject = 0;
              }
            }
          }
          if (pObject)
          {
            jr.setNode(pObject);
          }
        }
      }
      return jr;
    }

    AbsJson       AbsJson::set (std::string const & rhs)
    {
      return set(rhs.c_str());
    }

    AbsJson &     AbsJson::set (char const * key, bool rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (char const * key, int rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (char const * key, double rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (char const * key, char const * rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (char const * key, std::string const & rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (char const * key, AbsJson const & rhs)
    {
      set(key).assign(rhs);
      return *this;
    }

    AbsJson &     AbsJson::set (std::string const & key, bool rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::set (std::string const & key, int rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::set (std::string const & key, double rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::set (std::string const & key, char const * rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::set (std::string const & key, std::string const & rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::set (std::string const & key, AbsJson const & rhs)
    {
      return set(key.c_str(),rhs);
    }

    AbsJson &     AbsJson::push_back (bool rhs)
    {
      if (m_pNode)
      {
        cJSON * pObject = cJSON_CreateBool(rhs);
        if (pObject)
        {
          if (m_pNode->type != cJSON_Array)
          {
            private_clearNode(m_pNode);
            m_pNode->type = cJSON_Array;
          }
          cJSON_AddItemToArray(m_pNode,pObject);
          pObject = 0;
        }
      }
      return *this;
    }

    AbsJson &     AbsJson::push_back (int rhs)
    {
      if (m_pNode)
      {
        cJSON * pObject = cJSON_CreateNumber(rhs);
        if (pObject)
        {
          if (m_pNode->type != cJSON_Array)
          {
            private_clearNode(m_pNode);
            m_pNode->type = cJSON_Array;
          }
          cJSON_AddItemToArray(m_pNode,pObject);
          pObject = 0;
        }
      }
      return *this;
    }

    AbsJson &     AbsJson::push_back (double rhs)
    {
      if (m_pNode)
      {
        cJSON * pObject = cJSON_CreateNumber(rhs);
        if (pObject)
        {
          if (m_pNode->type != cJSON_Array)
          {
            private_clearNode(m_pNode);
            m_pNode->type = cJSON_Array;
          }
          cJSON_AddItemToArray(m_pNode,pObject);
          pObject = 0;
        }
      }
      return *this;
    }

    AbsJson &     AbsJson::push_back (char const * rhs)
    {
      if (rhs)
      {
        if (m_pNode)
        {
          cJSON * pObject = cJSON_CreateString(rhs);
          if (pObject)
          {
            if (m_pNode->type != cJSON_Array)
            {
              private_clearNode(m_pNode);
              m_pNode->type = cJSON_Array;
            }
            cJSON_AddItemToArray(m_pNode,pObject);
            pObject = 0;
          }
        }
      }
      return *this;
    }

    AbsJson &     AbsJson::push_back (std::string const & rhs)
    {
      return push_back(rhs.c_str());
    }

    AbsJson &     AbsJson::push_back (AbsJson const & rhs)
    {
      if (m_pNode && rhs.m_pNode && (m_pNode != rhs.m_pNode))
      {
        switch (rhs.m_pNode->type)
        {
        case cJSON_False:
          {
            push_back(false);
          }
          break;
        case cJSON_True:
          {
            push_back(true);
          }
          break;
        case cJSON_Number:
          {
            push_back(rhs.m_pNode->valuedouble);
          }
          break;
        case cJSON_String:
          {
            push_back(rhs.m_pNode->valuestring);
          }
          break;
        case cJSON_Array:
        case cJSON_Object:
          {
            cJSON * pObject = cJSON_Duplicate(rhs.m_pNode,1);
            if (pObject)
            {
              if (pObject->string)
              {
                cJSON_free(pObject->string);
                pObject->string = 0;
              }

              if (m_pNode->type != cJSON_Array)
              {
                private_clearNode(m_pNode);
                m_pNode->type = cJSON_Array;
              }

              cJSON_AddItemToArray(m_pNode,pObject);
              pObject = 0;
            }
          }
          break;
        default:
          {
            private_clearNode(m_pNode);
          }
          break;
        }
      }
      return *this;
    }

    int           AbsJson::length (void) const
    {
      int size = -1;
      if (m_pNode)
      {
        if ((m_pNode->type == cJSON_Array) || (m_pNode->type == cJSON_Object))
        {
          size = cJSON_GetArraySize(m_pNode);
        }
      }
      return size;
    }

    AbsJson const AbsJson::operator [] (int rhs) const
    {
      AbsJson jr;
      if (m_pNode)
      {
        if ((m_pNode->type == cJSON_Array) || (m_pNode->type == cJSON_Object))
        {
          cJSON * pObject = cJSON_GetArrayItem(m_pNode,rhs);
          if (pObject)
          {
            jr.setNode(pObject);
          }
        }
      }
      return jr;
    }

    AbsJson       AbsJson::operator [] (int rhs)
    {
      AbsJson jr;
      if (m_pNode)
      {
        if ((m_pNode->type == cJSON_Array) || (m_pNode->type == cJSON_Object))
        {
          cJSON * pObject = cJSON_GetArrayItem(m_pNode,rhs);
          if (pObject)
          {
            jr.setNode(pObject);
          }
        }
      }
      return jr;
    }

    AbsJson & AbsJson::assign (bool rhs)
    {
      if (m_pNode)
      {
        if ((m_pNode->type != cJSON_True) && (m_pNode->type != cJSON_False) && (m_pNode->type != cJSON_NULL))
        {
          private_clearNode(m_pNode);
        }
        m_pNode->type = ((rhs)?(cJSON_True):(cJSON_False));
      }
      return *this;
    }

    AbsJson & AbsJson::assign (int rhs)
    {
      if (m_pNode)
      {
        if ((m_pNode->type != cJSON_Number) && (m_pNode->type != cJSON_NULL))
        {
          private_clearNode(m_pNode);
        }
        m_pNode->type = cJSON_Number;
        cJSON_SetNumberValue(m_pNode,rhs);
      }
      return *this;
    }

    AbsJson & AbsJson::assign (double rhs)
    {
      if (m_pNode)
      {
        if ((m_pNode->type != cJSON_Number) && (m_pNode->type != cJSON_NULL))
        {
          private_clearNode(m_pNode);
        }
        m_pNode->type = cJSON_Number;
        cJSON_SetNumberValue(m_pNode,rhs);
      }
      return *this;
    }

    AbsJson & AbsJson::assign (char const * rhs)
    {
      if (rhs)
      {
        if (m_pNode)
        {
          if (m_pNode->type != cJSON_NULL)
          {
            private_clearNode(m_pNode);
          }
          m_pNode->type = cJSON_String;
          m_pNode->valuestring = cJSON_strdup(rhs);
        }
      }
      return *this;
    }

    AbsJson & AbsJson::assign (std::string const & rhs)
    {
      return assign(rhs.c_str());
    }

    AbsJson & AbsJson::assign (AbsJson const & rhs)
    {
      if (m_pNode && rhs.m_pNode && (m_pNode != rhs.m_pNode))
      {
        switch (rhs.m_pNode->type)
        {
        case cJSON_False:
          {
            assign(false);
          }
          break;
        case cJSON_True:
          {
            assign(true);
          }
          break;
        case cJSON_Number:
          {
            assign(rhs.m_pNode->valuedouble);
          }
          break;
        case cJSON_String:
          {
            assign(rhs.m_pNode->valuestring);
          }
          break;
        case cJSON_Array:
          {
            cJSON * pObject = cJSON_Duplicate(rhs.m_pNode,1);
            if (pObject)
            {
              private_clearNode(m_pNode);
              m_pNode->type = cJSON_Array;
              for (cJSON * pObjectChild = cJSON_DetachItemFromArray(pObject,0); pObjectChild; pObjectChild = cJSON_DetachItemFromArray(pObject,0))
              {
                cJSON_AddItemToArray(m_pNode,pObjectChild);
              }
              cJSON_Delete(pObject);
              pObject = 0;
            }
          }
          break;
        case cJSON_Object:
          {
            cJSON * pObject = cJSON_Duplicate(rhs.m_pNode,1);
            if (pObject)
            {
              private_clearNode(m_pNode);
              m_pNode->type = cJSON_Object;
              for (cJSON * pObjectChild = cJSON_DetachItemFromArray(pObject,0); pObjectChild; pObjectChild = cJSON_DetachItemFromArray(pObject,0))
              {
                cJSON_AddItemToArray(m_pNode,pObjectChild);
              }
              cJSON_Delete(pObject);
              pObject = 0;
            }
          }
          break;
        default:
          {
            private_clearNode(m_pNode);
          }
          break;
        }
      }
      return *this;
    }

    AbsJson & AbsJson::clear (void)
    {
      if (m_pNode)
      {
        if (m_pNode->type != cJSON_NULL)
        {
          private_clearNode(m_pNode);
        }
      }
      return *this;
    }
    AbsJson & AbsJson::remove (int rhs)
    {
      if (m_pNode)
      {
        if (m_pNode->type == cJSON_Array)
        {
          cJSON_DeleteItemFromArray(m_pNode,rhs);
        }
      }
      return *this;
    }

    AbsJson & AbsJson::remove (char const * rhs)
    {
      if (rhs)
      {
        if (m_pNode)
        {
          cJSON_DeleteItemFromObject(m_pNode,rhs);
        }
      }
      return *this;
    }

    AbsJson & AbsJson::remove (std::string const & rhs)
    {
      return remove(rhs.c_str());
    }

    bool AbsJson::isNull    (void) const
    {
      return ((m_pNode == 0) || (m_pNode->type == cJSON_NULL));
    }

    bool AbsJson::isBoolean (void) const
    {
      return ((m_pNode) && ((m_pNode->type == cJSON_True) || (m_pNode->type == cJSON_False)));
    }

    bool AbsJson::isNumber  (void) const
    {
      return ((m_pNode) && (m_pNode->type == cJSON_Number));
    }

    bool AbsJson::isString  (void) const
    {
      return ((m_pNode) && (m_pNode->type == cJSON_String));
    }

    bool AbsJson::isObject  (void) const
    {
      return ((m_pNode) && (m_pNode->type == cJSON_Object));
    }

    bool AbsJson::isArray   (void) const
    {
      return ((m_pNode) && (m_pNode->type == cJSON_Array));
    }

    bool AbsJson::isEmpty   (void) const
    {
      bool result = true;
      if (m_pNode)
      {
        switch (m_pNode->type)
        {
        case cJSON_False:
        case cJSON_True:
        case cJSON_Number:
          {
            result = false;
          }
          break;
        case cJSON_String:
          {
            if (m_pNode->valuestring)
            {
              result = (std::strlen(m_pNode->valuestring) < 1);
            }
          }
          break;
        case cJSON_Array:
        case cJSON_Object:
          {
            result = (!m_pNode->child);
          }
          break;
        default:
          {
          }
          break;
        }
      }
      return (result);
    }

    bool         AbsJson::asBoolean (void) const
    {
      bool result = false;
      if (m_pNode)
      {
        if (m_pNode->type == cJSON_True)
        {
          result = true;
        }
        else if (m_pNode->type == cJSON_Number)
        {
          result = m_pNode->valueint;
        }
        else if (m_pNode->type == cJSON_String)
        {
          char const * strTrue = CTrueString;
          char const * str = m_pNode->valuestring;
          if (str)
          {
            while (std::isspace(*str))
            {
              ++str;
            }
            while (((*str) != 0) && (std::tolower(*str) == (*strTrue)))
            {
              ++str; ++strTrue;
            }
            result = ((*str) == (*strTrue));
          }
        }
      }
      return result;
    }

    int          AbsJson::asInteger (void) const
    {
      int result = 0;
      if (m_pNode)
      {
        if (m_pNode->type == cJSON_True)
        {
          result = true;
        }
        else if (m_pNode->type == cJSON_False)
        {
          result = false;
        }
        else if (m_pNode->type == cJSON_Number)
        {
          result = m_pNode->valueint;
        }
        else if (m_pNode->type == cJSON_String)
        {
          if (m_pNode->valuestring)
          {
            result = std::atoi(m_pNode->valuestring);
          }
        }
      }
      return result;
    }

    double       AbsJson::asDouble  (void) const
    {
      double result = 0;
      if (m_pNode)
      {
        if (m_pNode->type == cJSON_True)
        {
          result = true;
        }
        else if (m_pNode->type == cJSON_False)
        {
          result = false;
        }
        else if (m_pNode->type == cJSON_Number)
        {
          result = m_pNode->valuedouble;
        }
        else if (m_pNode->type == cJSON_String)
        {
          if (m_pNode->valuestring)
          {
            result = std::atof(m_pNode->valuestring);
          }
        }
      }
      return result;
    }

    char const * AbsJson::asCString (void) const
    {
      char const * result = CEmptyString;
      if (m_pNode)
      {
        if (m_pNode->type == cJSON_String)
        {
          result = m_pNode->valuestring;
        }
      }
      return result;
    }

    std::string  AbsJson::asString  (void) const
    {
      return asCString();
    }

    bool AbsJson::parse (char const * cstr)
    {
      bool result = false;
      if (cstr)
      {
        cJSON * pObject = cJSON_Parse(cstr);
        if (pObject)
        {
          if (m_pNode)
          {
            private_clearNode(m_pNode);
            m_pNode->type = cJSON_Object;
            for (cJSON * pObjectChild = cJSON_DetachItemFromArray(pObject,0); pObjectChild; pObjectChild = cJSON_DetachItemFromArray(pObject,0))
            {
              cJSON_AddItemToArray(m_pNode,pObjectChild);
            }
            cJSON_Delete(pObject);
            pObject = 0;
          }
          else
          {
            m_pNode = pObject;
            pObject = 0;
          }
          result = true;
        }
      }
      return result;
    }

    bool AbsJson::parse (std::string const & str)
    {
      return parse(str.c_str());
    }

    void AbsJson::serialize (char ** pcstr) const
    {
      if (m_pNode && pcstr && (*pcstr) == 0)
      {
        (*pcstr) = cJSON_Print(m_pNode);
      }
    }

    void AbsJson::serialize (char * cstr, std::size_t n) const
    {
      if (cstr)
      {
        if (n > 1)
        {
          char * cstraux = 0;
          serialize(&cstraux);
          if (cstraux)
          {
            std::size_t cnt = std::strlen(cstraux);
            if (cnt < n)
            {
              std::strcpy(cstr,cstraux);
            }
            else
            {
              std::strncpy(cstr,cstraux,n-1);
              cstr[n] = 0;
            }
            cJSON_free(cstraux);
          }
        }
        else if (n == 1)
        {
          cstr[0] = 0;
        }
      }
    }

    void AbsJson::serialize (std::string & str) const
    {
      if (m_pNode)
      {
        char * cstr = 0;
        serialize(&cstr);
        if (cstr)
        {
          str = cstr;
          cJSON_free(cstr);
          cstr = 0;
        }
      }
    }

    void AbsJson::serialize (std::ostream & os) const
    {
      if (m_pNode)
      {
        char * cstr = 0;
        serialize(&cstr);
        if (cstr)
        {
          os << static_cast<char const *>(cstr);
          cJSON_free(cstr);
          cstr = 0;
        }
      }
    }

    std::string AbsJson::serialize (void) const
    {
      std::string result;
      serialize(result);
      return result;
    }

    AbsJson & AbsJson::operator = (bool rhs)
    {
      return assign(rhs);
    }

    AbsJson & AbsJson::operator = (int rhs)
    {
      return assign(rhs);
    }

    AbsJson & AbsJson::operator = (double rhs)
    {
      return assign(rhs);
    }

    AbsJson & AbsJson::operator = (char const * rhs)
    {
      return assign(rhs);
    }

    AbsJson & AbsJson::operator = (std::string const & rhs)
    {
      return assign(rhs);
    }

    AbsJson & AbsJson::operator = (AbsJson const & rhs)
    {
      return assign(rhs);
    }

    AbsJson::AbsJson (cJSON * pNode)
     : m_pNode(pNode)
    {
    }

    AbsJson::AbsJson (void)
     : m_pNode(0)
    {
    }

    void AbsJson::setNode (cJSON * pNode)
    {
      m_pNode = pNode;
    }

    void AbsJson::private_clearNode (cJSON * pNode)
    {
      if (pNode)
      {
        if (pNode->valuestring)
        {
          cJSON_free(pNode->valuestring);
          pNode->valuestring = 0;
        }
        pNode->valueint = 0;
        pNode->valuedouble = 0;
        for (cJSON * pObject = cJSON_DetachItemFromArray(pNode,0); pObject; pObject = cJSON_DetachItemFromArray(pNode,0))
        {
          cJSON_Delete(pObject);
        }
        pNode->type = cJSON_NULL;
      }
    }

    void AbsJson::private_destroyNode (cJSON * pObject)
    {
      cJSON_Delete(pObject);
    }

    cJSON * AbsJson::private_duplicateInternalNode (AbsJson const & rhs, bool clean = false)
    {
      cJSON * pObject = ((rhs.m_pNode)?(cJSON_Duplicate(rhs.m_pNode,1)):(0));
      if (pObject && clean)
      {
        if (pObject->valuestring)
        {
          cJSON_free(pObject->valuestring);
          pObject->valuestring = 0;
        }
      }
      return pObject;
    }

    cJSON * AbsJson::private_createNullInternalNode (void)
    {
      return cJSON_CreateNull();
    }
  }
}
