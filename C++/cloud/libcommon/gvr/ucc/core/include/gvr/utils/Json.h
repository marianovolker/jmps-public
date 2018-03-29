#ifndef JSON_H_
# define JSON_H_

# include <gvr/utils/AbsJson.h>

  namespace gvr
  {
    namespace utils
    {
      class Json : public AbsJson
      {
      public:
        virtual ~Json (void);

        Json (Json const &);

        Json (AbsJson const &);

        Json (void);
      };
    }
  }

#endif
