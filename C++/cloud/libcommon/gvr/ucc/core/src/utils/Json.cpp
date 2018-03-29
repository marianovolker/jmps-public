#include <gvr/utils/Json.h>

namespace gvr
{
  namespace utils
  {
    Json::~Json (void)
    {
      if (m_pNode)
      {
        private_destroyNode(m_pNode);
        m_pNode = 0;
      }
    }

    Json::Json (Json const & jr)
     : AbsJson(private_duplicateInternalNode(jr,true))
    {
    }

    Json::Json (AbsJson const & jr)
     : AbsJson(private_duplicateInternalNode(jr,true))
    {
    }

    Json::Json (void)
     : AbsJson(private_createNullInternalNode())
    {
    }
  }
}
