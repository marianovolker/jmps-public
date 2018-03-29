#ifndef ABSJSON_H_
# define ABSJSON_H_

# include <string>
# include <ostream>

  class cJSON;

  namespace gvr
  {
    namespace utils
    {
      class AbsJson
      {
      public:
        virtual ~AbsJson (void);

        AbsJson (AbsJson const &);

        virtual char const * name (void) const;

        virtual AbsJson const get (char const *) const;
        virtual AbsJson const get (std::string const &) const;
        virtual AbsJson       get (char const *);
        virtual AbsJson       get (std::string const &);

        virtual AbsJson       set (char const *);
        virtual AbsJson       set (std::string const &);

        virtual AbsJson &     set (char const *, bool);
        virtual AbsJson &     set (char const *, int);
        virtual AbsJson &     set (char const *, double);
        virtual AbsJson &     set (char const *, char const *);
        virtual AbsJson &     set (char const *, std::string const &);
        virtual AbsJson &     set (char const *, AbsJson const &);

        virtual AbsJson &     set (std::string const &, bool);
        virtual AbsJson &     set (std::string const &, int);
        virtual AbsJson &     set (std::string const &, double);
        virtual AbsJson &     set (std::string const &, char const *);
        virtual AbsJson &     set (std::string const &, std::string const &);
        virtual AbsJson &     set (std::string const &, AbsJson const &);

        virtual AbsJson &     push_back (bool);
        virtual AbsJson &     push_back (int);
        virtual AbsJson &     push_back (double);
        virtual AbsJson &     push_back (char const *);
        virtual AbsJson &     push_back (std::string const &);
        virtual AbsJson &     push_back (AbsJson const &);

        virtual int           length       (void) const;
        virtual AbsJson const operator [] (int) const;
        virtual AbsJson       operator [] (int);

        virtual AbsJson & assign (bool);
        virtual AbsJson & assign (int);
        virtual AbsJson & assign (double);
        virtual AbsJson & assign (char const *);
        virtual AbsJson & assign (std::string const &);
        virtual AbsJson & assign (AbsJson const &);

        virtual AbsJson & clear (void);
        virtual AbsJson & remove (int);
        virtual AbsJson & remove (char const *);
        virtual AbsJson & remove (std::string const &);

        virtual bool isNull    (void) const;
        virtual bool isBoolean (void) const;
        virtual bool isNumber  (void) const;
        virtual bool isString  (void) const;
        virtual bool isObject  (void) const;
        virtual bool isArray   (void) const;

        virtual bool isEmpty   (void) const;

        virtual bool         asBoolean (void) const;
        virtual int          asInteger (void) const;
        virtual double       asDouble  (void) const;
        virtual char const * asCString (void) const;
        virtual std::string  asString  (void) const;

        virtual bool parse (char const *);
        virtual bool parse (std::string const &);
        virtual void serialize (char **) const;
        virtual void serialize (char *, std::size_t) const;
        virtual void serialize (std::string &) const;
        virtual void serialize (std::ostream &) const;

        virtual std::string serialize (void) const;

        AbsJson & operator = (bool);
        AbsJson & operator = (int);
        AbsJson & operator = (double);
        AbsJson & operator = (char const *);
        AbsJson & operator = (std::string const &);
        AbsJson & operator = (AbsJson const &);

      protected:
        AbsJson (cJSON *);
        AbsJson (void);

        void setNode (cJSON *);

        static void private_clearNode (cJSON *);
        static void private_destroyNode (cJSON *);
        static cJSON * private_duplicateInternalNode (AbsJson const &, bool);
        static cJSON * private_createNullInternalNode (void);

        cJSON * m_pNode;
      };
    }
  }

#endif
