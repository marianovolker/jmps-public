#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.DataStore2 
   :platform: Unix, Windows
   :synopsis: Utility classes for data store (Python 2).

This module contains some "utility classes" to managing the data store.

"""

import os, sys, re, binascii

from collections import MutableMapping
#from collections import OrderedDict as _default_dict, ChainMap as _ChainMap
from collections import OrderedDict as _default_dict
import functools
import io
import itertools
import warnings

try:
	from means import *
except:
	from .means import *

__all__ = ["NoSectionError", "DuplicateOptionError", "DuplicateSectionError",
           "NoOptionError", "InterpolationError", "InterpolationDepthError",
           "InterpolationSyntaxError", "ParsingError",
           "MissingSectionHeaderError",
           "ConfigParser", "SafeConfigParser", "RawConfigParser",
           "DEFAULTSECT", "MAX_INTERPOLATION_DEPTH"]

DEFAULTSECT = "DEFAULT"

MAX_INTERPOLATION_DEPTH = 10

import UserDict

class _ChainMap(MutableMapping):
    """ 
    A *ChainMap* groups multiple dicts (or other mappings) together to create a single, updatable view.
    The underlying mappings are stored in a list.  That list is public and can accessed or updated using the *maps* attribute. There is no other state.
    Lookups search the underlying mappings successively until a key is found. In contrast, writes, updates, and deletions only operate on the first mapping.

    The underlying mappings are stored in a list.  That list is public and can
    accessed or updated using the *maps* attribute.  There is no other state.

    Lookups search the underlying mappings successively until a key is found.
    In contrast, writes, updates, and deletions only operate on the first
    mapping.

    """

    def __init__(self, *maps):
        """
        Initialize a ChainMap by setting *maps* to the given mappings.
        If no mappings are provided, a single empty dictionary is used.

        :Parameters:
        	- *\*maps* (Map): Maps to handle. 
        
        """
        
        self.maps = list(maps) or [{}]          # always at least one map

    def __missing__(self, key):
        raise KeyError(key)

    def __getitem__(self, key):
        for mapping in self.maps:
            try:
                return mapping[key]             # can't use 'key in mapping' with defaultdict
            except KeyError:
                pass
        return self.__missing__(key)            # support subclasses that define __missing__

    def get(self, key, default=None):
        return self[key] if key in self else default

    def __len__(self):
        return len(set().union(*self.maps))     # reuses stored hash values if possible

    def __iter__(self):
        return iter(set().union(*self.maps))

    def __contains__(self, key):
        return any(key in m for m in self.maps)

    def __bool__(self):
        return any(self.maps)

#   @_recursive_repr()
    def __repr__(self):
        return '{0.__class__.__name__}({1})'.format(
            self, ', '.join(map(repr, self.maps)))

    @classmethod
    def fromkeys(cls, iterable, *args):
        """
        Create a ChainMap with a single dict created from the iterable.
        
        :Parameters:
			- *iterable* (): STBO: chiedere.
			- *\*args* (): STBO: chiedere. 
        
        :Returns: STBO:chiedere.
        
        """
        return cls(dict.fromkeys(iterable, *args))

    def copy(self):
        """
        New ChainMap or subclass with a new copy of maps[0] and refs to maps[1:]
        
        :Returns: TODO.
        
        """
        return self.__class__(self.maps[0].copy(), *self.maps[1:])

    __copy__ = copy

    def new_child(self):                        # like Django's Context.push()
        """
        New *ChainMap* with a new dict followed by all previous maps.
        
        :Returns: TODO.
        
        """
        return self.__class__({}, *self.maps)

    @property
    def parents(self):                          # like Django's Context.pop()
        'New ChainMap from maps[1:].'
        return self.__class__(*self.maps[1:])

    def __setitem__(self, key, value):
        self.maps[0][key] = value

    def __delitem__(self, key):
        try:
            del self.maps[0][key]
        except KeyError:
            raise KeyError('Key not found in the first mapping: {!r}'.format(key))

    def popitem(self):
        """
        Removes and returns an item pair from ``maps[0]``. Raise *KeyError* is ``maps[0]`` is empty.
        
        :Raises: *KeyError*.
        
        """
        try:
            return self.maps[0].popitem()
        except KeyError:
            raise KeyError('No keys found in the first mapping.')

    def pop(self, key, *args):
        """
        Removes *key* from ``maps[0]`` and return its value. 
        
        :Raises: *KeyError* if *key* not in ``maps[0]``.
        
        """
        try:
            return self.maps[0].pop(key, *args)
        except KeyError:
            raise KeyError('Key not found in the first mapping: {!r}'.format(key))

    def clear(self):
        """
        Clears ``maps[0], leaving ``maps[1:]`` intact.
        
        """
        self.maps[0].clear()

class ChainMap(UserDict.DictMixin):
    """
    This class combines multiple mappings for sequential lookup.

    For example, to emulate Python's normal lookup sequence::

        import __builtin__
        pylookup = Chainmap(locals(), globals(), vars(__builtin__))        
    """

    def __init__(self, *maps):
    	"""
        Initialize a ChainMap by setting *maps* to the given mappings.
        If no mappings are provided, a single empty dictionary is used.
        
        :Parameters:
        	- *\*maps* (Map): Maps to handle. 
        
        """
        self._maps = maps

    def __getitem__(self, key):
        for mapping in self._maps:
            try:
                return mapping[key]
            except KeyError:
                pass
        raise KeyError(key)

# exception classes
class Error(Exception):
    """
    Base class for *ConfigParser* exceptions.
    
    """

    def _get_message(self):
        """
        Getter for "message"; needed only to override deprecation in BaseException.
        
        """
        return self.__message

    def _set_message(self, value):
        """
        Setter for "message"; needed only to override deprecation in *BaseException*.
        
        """
        self.__message = value

    # BaseException.message has been deprecated since Python 2.6.  To prevent
    # DeprecationWarning from popping up over this pre-existing attribute, use
    # a new property that takes lookup precedence.
    message = property(_get_message, _set_message)

    def __init__(self, msg=''):
        self.message = msg
        Exception.__init__(self, msg)

    def __repr__(self):
        return self.message

    __str__ = __repr__


class NoSectionError(Error):
    """
    Raised when no section matches a requested option.
    
    """

    def __init__(self, section):
        Error.__init__(self, 'No section: %r' % (section,))
        self.section = section
        self.args = (section, )


class DuplicateSectionError(Error):
    """
    Raised when a section is repeated in an input source.
    Possible repetitions that raise this exception are: multiple creation using the API or in strict parsers when a section is found more than once in a single input file, string or dictionary.
    """

    def __init__(self, section, source=None, lineno=None):
        msg = [repr(section), " already exists"]
        if source is not None:
            message = ["While reading from ", source]
            if lineno is not None:
                message.append(" [line {0:2d}]".format(lineno))
            message.append(": section ")
            message.extend(msg)
            msg = message
        else:
            msg.insert(0, "Section ")
        Error.__init__(self, "".join(msg))
        self.section = section
        self.source = source
        self.lineno = lineno
        self.args = (section, source, lineno)


class DuplicateOptionError(Error):
    """
    Raised by strict parsers when an option is repeated in an input source.
    Current implementation raises this exception only when an option is found more than once in a single file, string or dictionary.
    """

    def __init__(self, section, option, source=None, lineno=None):
        msg = [repr(option), " in section ", repr(section),
               " already exists"]
        if source is not None:
            message = ["While reading from ", source]
            if lineno is not None:
                message.append(" [line {0:2d}]".format(lineno))
            message.append(": option ")
            message.extend(msg)
            msg = message
        else:
            msg.insert(0, "Option ")
        Error.__init__(self, "".join(msg))
        self.section = section
        self.option = option
        self.source = source
        self.lineno = lineno
        self.args = (section, option, source, lineno)


class NoOptionError(Error):
    """
    A requested option was not found.
    
    """

    def __init__(self, option, section):
        Error.__init__(self, "No option %r in section: %r" %
                       (option, section))
        self.option = option
        self.section = section
        self.args = (option, section)


class InterpolationError(Error):
    """
    Base class for interpolation\-related exceptions.
    
    """

    def __init__(self, option, section, msg):
        Error.__init__(self, msg)
        self.option = option
        self.section = section
        self.args = (option, section, msg)


class InterpolationMissingOptionError(InterpolationError):
    """
    A string substitution required a setting which was not available.
    
    """

    def __init__(self, option, section, rawval, reference):
        msg = ("Bad value substitution:\n"
               "\tsection: [%s]\n"
               "\toption : %s\n"
               "\tkey    : %s\n"
               "\trawval : %s\n"
               % (section, option, reference, rawval))
        InterpolationError.__init__(self, option, section, msg)
        self.reference = reference
        self.args = (option, section, rawval, reference)


class InterpolationSyntaxError(InterpolationError):
    """
    Raised when the source text contains invalid syntax.
    Current implementation raises this exception when the source text into which substitutions are made does not conform to the required syntax.

    """


class InterpolationDepthError(InterpolationError):
    """
    Raised when substitutions are nested too deeply.
    
    """

    def __init__(self, option, section, rawval):
        msg = ("Value interpolation too deeply recursive:\n"
               "\tsection: [%s]\n"
               "\toption : %s\n"
               "\trawval : %s\n"
               % (section, option, rawval))
        InterpolationError.__init__(self, option, section, msg)
        self.args = (option, section, rawval)


class ParsingError(Error):
    """
    Raised when a configuration file does not follow legal syntax.
    
    """

    def __init__(self, source=None, filename=None):
        # Exactly one of `source'/`filename' arguments has to be given.
        # `filename' kept for compatibility.
        if filename and source:
            raise ValueError("Cannot specify both `filename' and `source'. "
                             "Use `source'.")
        elif not filename and not source:
            raise ValueError("Required argument `source' not given.")
        elif filename:
            source = filename
        Error.__init__(self, 'Source contains parsing errors: %s' % source)
        self.source = source
        self.errors = []
        self.args = (source, )

    @property
    def filename(self):
        """
        Deprecated, use `source'.
        
        """
        warnings.warn(
            "The 'filename' attribute will be removed in future versions.  "
            "Use 'source' instead.",
            DeprecationWarning, stacklevel=2
        )
        return self.source

    @filename.setter
    def filename(self, value):
        """
        Deprecated, user `source'.
        
        """
        warnings.warn(
            "The 'filename' attribute will be removed in future versions.  "
            "Use 'source' instead.",
            DeprecationWarning, stacklevel=2
        )
        self.source = value

    def append(self, lineno, line):
        self.errors.append((lineno, line))
        self.message += '\n\t[line %2d]: %s' % (lineno, line)


class MissingSectionHeaderError(ParsingError):
    """
    Raised when a key-value pair is found before any section header.
    
    """

    def __init__(self, filename, lineno, line):
        Error.__init__(
            self,
            'File contains no section headers.\nfile: %s, line: %d\n%r' %
            (filename, lineno, line))
        self.source = filename
        self.lineno = lineno
        self.line = line
        self.args = (filename, lineno, line)


# Used in parser getters to indicate the default behaviour when a specific
# option is not found it to raise an exception. Created to enable `None' as
# a valid fallback value.
_UNSET = object()


class Interpolation:
    """
    Dummy interpolation that passes the value through with no changes.
    
    """

    def before_get(self, parser, section, option, value, defaults):
        return value

    def before_set(self, parser, section, option, value):
        return value

    def before_read(self, parser, section, option, value):
        return value

    def before_write(self, parser, section, option, value):
        return value


class BasicInterpolation(Interpolation):
    """
    Interpolation as implemented in the classic ConfigParser.
	The option values can contain format strings which refer to other values in the same section, or values in the special default section.

    For example:

        something: %(dir)s/whatever

    would resolve the "%(dir)s" to the value of dir.  All reference expansions are done late, on demand. 
    If a user needs to use a bare % in a configuration file, she can escape it by writing %%. Other % usage is considered a user error and raises `InterpolationSyntaxError'.
    """

    _KEYCRE = re.compile(r"%\(([^)]+)\)s")

    def before_get(self, parser, section, option, value, defaults):
        L = []
        self._interpolate_some(parser, option, L, value, section, defaults, 1)
        return ''.join(L)

    def before_set(self, parser, section, option, value):
        tmp_value = value.replace('%%', '') # escaped percent signs
        tmp_value = self._KEYCRE.sub('', tmp_value) # valid syntax
        if '%' in tmp_value:
            raise ValueError("invalid interpolation syntax in %r at "
                             "position %d" % (value, tmp_value.find('%')))
        return value

    def _interpolate_some(self, parser, option, accum, rest, section, map,
                          depth):
        if depth > MAX_INTERPOLATION_DEPTH:
            raise InterpolationDepthError(option, section, rest)
        while rest:
            p = rest.find("%")
            if p < 0:
                accum.append(rest)
                return
            if p > 0:
                accum.append(rest[:p])
                rest = rest[p:]
            # p is no longer used
            c = rest[1:2]
            if c == "%":
                accum.append("%")
                rest = rest[2:]
            elif c == "(":
                m = self._KEYCRE.match(rest)
                if m is None:
                    raise InterpolationSyntaxError(option, section,
                        "bad interpolation variable reference %r" % rest)
                var = parser.optionxform(m.group(1))
                rest = rest[m.end():]
                try:
                    v = map[var]
                except KeyError:
                    raise InterpolationMissingOptionError(
                        option, section, rest, var)
                if "%" in v:
                    self._interpolate_some(parser, option, accum, v,
                                           section, map, depth + 1)
                else:
                    accum.append(v)
            else:
                raise InterpolationSyntaxError(
                    option, section,
                    "'%%' must be followed by '%%' or '(', "
                    "found: %r" % (rest,))


class ExtendedInterpolation(Interpolation):
    """
    Advanced variant of interpolation, supports the syntax used by ``zc.buildout``. 
    Enables interpolation between sections.
    
    """

    _KEYCRE = re.compile(r"\$\{([^}]+)\}")

    def before_get(self, parser, section, option, value, defaults):
        L = []
        self._interpolate_some(parser, option, L, value, section, defaults, 1)
        return ''.join(L)

    def before_set(self, parser, section, option, value):
        tmp_value = value.replace('$$', '') # escaped dollar signs
        tmp_value = self._KEYCRE.sub('', tmp_value) # valid syntax
        if '$' in tmp_value:
            raise ValueError("invalid interpolation syntax in %r at "
                             "position %d" % (value, tmp_value.find('%')))
        return value

    def _interpolate_some(self, parser, option, accum, rest, section, map,
                          depth):
        if depth > MAX_INTERPOLATION_DEPTH:
            raise InterpolationDepthError(option, section, rest)
        while rest:
            p = rest.find("$")
            if p < 0:
                accum.append(rest)
                return
            if p > 0:
                accum.append(rest[:p])
                rest = rest[p:]
            # p is no longer used
            c = rest[1:2]
            if c == "$":
                accum.append("$")
                rest = rest[2:]
            elif c == "{":
                m = self._KEYCRE.match(rest)
                if m is None:
                    raise InterpolationSyntaxError(option, section,
                        "bad interpolation variable reference %r" % rest)
                path = m.group(1).split(':')
                rest = rest[m.end():]
                sect = section
                opt = option
                try:
                    if len(path) == 1:
                        opt = parser.optionxform(path[0])
                        v = map[opt]
                    elif len(path) == 2:
                        sect = path[0]
                        opt = parser.optionxform(path[1])
                        v = parser.get(sect, opt, raw=True)
                    else:
                        raise InterpolationSyntaxError(
                            option, section,
                            "More than one ':' found: %r" % (rest,))
                except (KeyError, NoSectionError, NoOptionError):
                    raise InterpolationMissingOptionError(
                        option, section, rest, ":".join(path))
                if "$" in v:
                    self._interpolate_some(parser, opt, accum, v, sect,
                                           dict(parser.items(sect, raw=True)),
                                           depth + 1)
                else:
                    accum.append(v)
            else:
                raise InterpolationSyntaxError(
                    option, section,
                    "'$' must be followed by '$' or '{', "
                    "found: %r" % (rest,))


class LegacyInterpolation(Interpolation):
    """
    Deprecated interpolation used in old versions of ConfigParser.
    Use *BasicInterpolation* or *ExtendedInterpolation* instead.
    
    """

    _KEYCRE = re.compile(r"%\(([^)]*)\)s|.")

    def before_get(self, parser, section, option, value, vars):
        rawval = value
        depth = MAX_INTERPOLATION_DEPTH
        while depth:                    # Loop through this until it's done
            depth -= 1
            if value and "%(" in value:
                replace = functools.partial(self._interpolation_replace,
                                            parser=parser)
                value = self._KEYCRE.sub(replace, value)
                try:
                    value = value % vars
                except KeyError as e:
                    raise InterpolationMissingOptionError(
                        option, section, rawval, e.args[0])
            else:
                break
        if value and "%(" in value:
            raise InterpolationDepthError(option, section, rawval)
        return value

    def before_set(self, parser, section, option, value):
        return value

    @staticmethod
    def _interpolation_replace(match, parser):
        s = match.group(1)
        if s is None:
            return match.group()
        else:
            return "%%(%s)s" % parser.optionxform(s)


class RawConfigParser(MutableMapping):
    """
    *ConfigParser* that does not do interpolation.
    
    """

    # Regular expressions for parsing section headers and options
    _SECT_TMPL = r"""
        \[                                 # [
        (?P<header>[^]]+)                  # very permissive!
        \]                                 # ]
        """
    _OPT_TMPL = r"""
        (?P<option>.*?)                    # very permissive!
        \s*(?P<vi>{delim})\s*              # any number of space/tab,
                                           # followed by any of the
                                           # allowed delimiters,
                                           # followed by any space/tab
        (?P<value>.*)$                     # everything up to eol
        """
    _OPT_NV_TMPL = r"""
        (?P<option>.*?)                    # very permissive!
        \s*(?:                             # any number of space/tab,
        (?P<vi>{delim})\s*                 # optionally followed by
                                           # any of the allowed
                                           # delimiters, followed by any
                                           # space/tab
        (?P<value>.*))?$                   # everything up to eol
        """
    # Interpolation algorithm to be used if the user does not specify another
    _DEFAULT_INTERPOLATION = Interpolation()
    # Compiled regular expression for matching sections
    SECTCRE = re.compile(_SECT_TMPL, re.VERBOSE)
    # Compiled regular expression for matching options with typical separators
    OPTCRE = re.compile(_OPT_TMPL.format(delim="=|:"), re.VERBOSE)
    # Compiled regular expression for matching options with optional values
    # delimited using typical separators
    OPTCRE_NV = re.compile(_OPT_NV_TMPL.format(delim="=|:"), re.VERBOSE)
    # Compiled regular expression for matching leading whitespace in a line
    NONSPACECRE = re.compile(r"\S")
    # Possible boolean values in the configuration.
    BOOLEAN_STATES = {'1': True, 'yes': True, 'true': True, 'on': True,
                      '0': False, 'no': False, 'false': False, 'off': False}

    def __init__(self, defaults=None, dict_type=_default_dict,
                 allow_no_value=False, delimiters=('=', ':'),
                 comment_prefixes=('#', ';'), inline_comment_prefixes=None,
                 strict=True, empty_lines_in_values=True,
                 default_section=DEFAULTSECT,
                 interpolation=_UNSET):

        self._dict = dict_type
        self._sections = self._dict()
        self._defaults = self._dict()
        self._proxies = self._dict()
        self._proxies[default_section] = SectionProxy(self, default_section)
        if defaults:
            for key, value in defaults.items():
                self._defaults[self.optionxform(key)] = value
        self._delimiters = tuple(delimiters)
        if delimiters == ('=', ':'):
            self._optcre = self.OPTCRE_NV if allow_no_value else self.OPTCRE
        else:
            d = "|".join(re.escape(d) for d in delimiters)
            if allow_no_value:
                self._optcre = re.compile(self._OPT_NV_TMPL.format(delim=d),
                                          re.VERBOSE)
            else:
                self._optcre = re.compile(self._OPT_TMPL.format(delim=d),
                                          re.VERBOSE)
        self._comment_prefixes = tuple(comment_prefixes or ())
        self._inline_comment_prefixes = tuple(inline_comment_prefixes or ())
        self._strict = strict
        self._allow_no_value = allow_no_value
        self._empty_lines_in_values = empty_lines_in_values
        self.default_section=default_section
        self._interpolation = interpolation
        if self._interpolation is _UNSET:
            self._interpolation = self._DEFAULT_INTERPOLATION
        if self._interpolation is None:
            self._interpolation = Interpolation()

    def defaults(self):
        return self._defaults

    def sections(self):
        """
        Return a list of section names, excluding [DEFAULT].
        
        """
        # self._sections will never have [DEFAULT] in it
        return list(self._sections.keys())

    def add_section(self, section):
        """
        Create a new section in the configuration.
		Raise DuplicateSectionError if a section by the specified name already exists. Raise ValueError if name is DEFAULT.
        """
        if section == self.default_section:
            raise ValueError('Invalid section name: %r' % section)

        if section in self._sections:
            raise DuplicateSectionError(section)
        self._sections[section] = self._dict()
        self._proxies[section] = SectionProxy(self, section)

    def has_section(self, section):
        """Indicate whether the named section is present in the configuration.

        The DEFAULT section is not acknowledged.
        """
        return section in self._sections

    def options(self, section):
        """Return a list of option names for the given section name."""
        try:
            opts = self._sections[section].copy()
        except KeyError:
            raise NoSectionError(section)
        opts.update(self._defaults)
        return list(opts.keys())

    def read(self, filenames):
        """Read and parse a filename or a list of filenames.

        Files that cannot be opened are silently ignored; this is
        designed so that you can specify a list of potential
        configuration file locations (e.g. current directory, user's
        home directory, systemwide directory), and all existing
        configuration files in the list will be read.  A single
        filename may also be given.

        Return list of successfully read files.
        """
        if isinstance(filenames, str):
            filenames = [filenames]
        read_ok = []
        for filename in filenames:
            try:
                with open(filename) as fp:
                    self._read(fp, filename)
            except IOError:
                continue
            read_ok.append(filename)
        return read_ok

    def read_file(self, f, source=None):
        """Like read() but the argument must be a file-like object.

        The `f' argument must be iterable, returning one line at a time.
        Optional second argument is the `source' specifying the name of the
        file being read. If not given, it is taken from f.name. If `f' has no
        `name' attribute, `<???>' is used.
        """
        if source is None:
            try:
                source = f.name
            except AttributeError:
                source = '<???>'
        self._read(f, source)

    def read_string(self, string, source='<string>'):
        """Read configuration from a given string."""
        sfile = io.StringIO(string)
        self.read_file(sfile, source)

    def read_dict(self, dictionary, source='<dict>'):
        """Read configuration from a dictionary.

        Keys are section names, values are dictionaries with keys and values
        that should be present in the section. If the used dictionary type
        preserves order, sections and their keys will be added in order.

        All types held in the dictionary are converted to strings during
        reading, including section names, option names and keys.

        Optional second argument is the `source' specifying the name of the
        dictionary being read.
        """
        elements_added = set()
        for section, keys in dictionary.items():
            section = str(section)
            try:
                self.add_section(section)
            except (DuplicateSectionError, ValueError):
                if self._strict and section in elements_added:
                    raise
            elements_added.add(section)
            for key, value in keys.items():
                key = self.optionxform(str(key))
                if value is not None:
                    value = str(value)
                if self._strict and (section, key) in elements_added:
                    raise DuplicateOptionError(section, key, source)
                elements_added.add((section, key))
                self.set(section, key, value)

    def readfp(self, fp, filename=None):
        """Deprecated, use read_file instead."""
        warnings.warn(
            "This method will be removed in future versions.  "
            "Use 'parser.read_file()' instead.",
            DeprecationWarning, stacklevel=2
        )
        self.read_file(fp, source=filename)

    def get(self, section, option, raw=False, vars=None, fallback=_UNSET):
        """Get an option value for a given section.

        If `vars' is provided, it must be a dictionary. The option is looked up
        in `vars' (if provided), `section', and in `DEFAULTSECT' in that order.
        If the key is not found and `fallback' is provided, it is used as
        a fallback value. `None' can be provided as a `fallback' value.

        If interpolation is enabled and the optional argument `raw' is False,
        all interpolations are expanded in the return values.

        Arguments `raw', `vars', and `fallback' are keyword only.

        The section DEFAULT is special.
        """
        try:
            d = self._unify_values(section, vars)
        except NoSectionError:
            if fallback is _UNSET:
                raise
            else:
                return fallback
        option = self.optionxform(option)
        try:
            value = d[option]
        except KeyError:
            if fallback is _UNSET:
                raise NoOptionError(option, section)
            else:
                return fallback

        if raw or value is None:
            return value
        else:
            return self._interpolation.before_get(self, section, option, value,
                                                  d)

    def _get(self, section, conv, option, **kwargs):
        return conv(self.get(section, option, **kwargs))

    def getint(self, section, option, raw=False, vars=None,
               fallback=_UNSET):
        try:
            return self._get(section, int, option, raw=raw, vars=vars)
        except (NoSectionError, NoOptionError):
            if fallback is _UNSET:
                raise
            else:
                return fallback

    def getfloat(self, section, option, raw=False, vars=None,
                 fallback=_UNSET):
        try:
            return self._get(section, float, option, raw=raw, vars=vars)
        except (NoSectionError, NoOptionError):
            if fallback is _UNSET:
                raise
            else:
                return fallback

    def getboolean(self, section, option, raw=False, vars=None,
                   fallback=_UNSET):
        try:
            return self._get(section, self._convert_to_boolean, option,
                             raw=raw, vars=vars)
        except (NoSectionError, NoOptionError):
            if fallback is _UNSET:
                raise
            else:
                return fallback

    def items(self, section=_UNSET, raw=False, vars=None):
        """Return a list of (name, value) tuples for each option in a section.

        All % interpolations are expanded in the return values, based on the
        defaults passed into the constructor, unless the optional argument
        `raw' is true.  Additional substitutions may be provided using the
        `vars' argument, which must be a dictionary whose contents overrides
        any pre-existing defaults.

        The section DEFAULT is special.
        """
        if section is _UNSET:
            return super().items()
        d = self._defaults.copy()
        try:
            d.update(self._sections[section])
        except KeyError:
            if section != self.default_section:
                raise NoSectionError(section)
        # Update with the entry specific variables
        if vars:
            for key, value in vars.items():
                d[self.optionxform(key)] = value
        value_getter = lambda option: self._interpolation.before_get(self,
            section, option, d[option], d)
        if raw:
            value_getter = lambda option: d[option]
        return [(option, value_getter(option)) for option in d.keys()]

    def popitem(self):
        """Remove a section from the parser and return it as
        a (section_name, section_proxy) tuple. If no section is present, raise
        KeyError.

        The section DEFAULT is never returned because it cannot be removed.
        """
        for key in self.sections():
            value = self[key]
            del self[key]
            return key, value
        raise KeyError

    def optionxform(self, optionstr):
        return optionstr.lower()

    def has_option(self, section, option):
        """Check for the existence of a given option in a given section.
        If the specified `section' is None or an empty string, DEFAULT is
        assumed. If the specified `section' does not exist, returns False."""
        if not section or section == self.default_section:
            option = self.optionxform(option)
            return option in self._defaults
        elif section not in self._sections:
            return False
        else:
            option = self.optionxform(option)
            return (option in self._sections[section]
                    or option in self._defaults)

    def set(self, section, option, value=None):
        """Set an option."""
        if value:
            value = self._interpolation.before_set(self, section, option,
                                                   value)
        if not section or section == self.default_section:
            sectdict = self._defaults
        else:
            try:
                sectdict = self._sections[section]
            except KeyError:
                raise NoSectionError(section)
        sectdict[self.optionxform(option)] = value

    def write(self, fp, space_around_delimiters=True):
        """Write an .ini-format representation of the configuration state.

        If `space_around_delimiters' is True (the default), delimiters
        between keys and values are surrounded by spaces.
        """
        if space_around_delimiters:
            d = " {} ".format(self._delimiters[0])
        else:
            d = self._delimiters[0]
        if self._defaults:
            self._write_section(fp, self.default_section,
                                    self._defaults.items(), d)
        for section in self._sections:
            self._write_section(fp, section,
                                self._sections[section].items(), d)

    def _write_section(self, fp, section_name, section_items, delimiter):
        """Write a single section to the specified `fp'."""
        fp.write("[{}]\n".format(section_name))
        for key, value in section_items:
            value = self._interpolation.before_write(self, section_name, key,
                                                     value)
            if value is not None or not self._allow_no_value:
                value = delimiter + str(value).replace('\n', '\n\t')
            else:
                value = ""
            fp.write("{}{}\n".format(key, value))
        fp.write("\n")

    def remove_option(self, section, option):
        """Remove an option."""
        if not section or section == self.default_section:
            sectdict = self._defaults
        else:
            try:
                sectdict = self._sections[section]
            except KeyError:
                raise NoSectionError(section)
        option = self.optionxform(option)
        existed = option in sectdict
        if existed:
            del sectdict[option]
        return existed

    def remove_section(self, section):
        """Remove a file section."""
        existed = section in self._sections
        if existed:
            del self._sections[section]
            del self._proxies[section]
        return existed

    def __getitem__(self, key):
        if key != self.default_section and not self.has_section(key):
            raise KeyError(key)
        return self._proxies[key]

    def __setitem__(self, key, value):
        # To conform with the mapping protocol, overwrites existing values in
        # the section.

        # XXX this is not atomic if read_dict fails at any point. Then again,
        # no update method in configparser is atomic in this implementation.
        if key == self.default_section:
            self._defaults.clear()
        elif key in self._sections:
            self._sections[key].clear()
        self.read_dict({key: value})

    def __delitem__(self, key):
        if key == self.default_section:
            raise ValueError("Cannot remove the default section.")
        if not self.has_section(key):
            raise KeyError(key)
        self.remove_section(key)

    def __contains__(self, key):
        return key == self.default_section or self.has_section(key)

    def __len__(self):
        return len(self._sections) + 1 # the default section

    def __iter__(self):
        # XXX does it break when underlying container state changed?
        return itertools.chain((self.default_section,), self._sections.keys())

    def _read(self, fp, fpname):
        """
        Parse a sectioned configuration file.
        Each section in a configuration file contains a header, indicated by a name in square brackets (`[]'), plus key/value options, indicated by `name' and `value' delimited with a specific substring (`=' or `:' by default).
        Values can span multiple lines, as long as they are indented deeper than the first line of the value. Depending on the parser's mode, blank lines may be treated as parts of multiline values or ignored.
        Configuration files may include comments, prefixed by specific characters (`#' and `;' by default). 
        Comments may appear on their own in an otherwise empty line or may be entered in lines holding values or section names.
        
        """
        elements_added = set()
        cursect = None                        # None, or a dictionary
        sectname = None
        optname = None
        lineno = 0
        indent_level = 0
        e = None                              # None, or an exception
        for lineno, line in enumerate(fp, start=1):
            comment_start = sys.maxsize
            # strip inline comments
            inline_prefixes = {p: -1 for p in self._inline_comment_prefixes}
            while comment_start == sys.maxsize and inline_prefixes:
                next_prefixes = {}
                for prefix, index in inline_prefixes.items():
                    index = line.find(prefix, index+1)
                    if index == -1:
                        continue
                    next_prefixes[prefix] = index
                    if index == 0 or (index > 0 and line[index-1].isspace()):
                        comment_start = min(comment_start, index)
                inline_prefixes = next_prefixes
            # strip full line comments
            for prefix in self._comment_prefixes:
                if line.strip().startswith(prefix):
                    comment_start = 0
                    break
            if comment_start == sys.maxsize:
                comment_start = None
            value = line[:comment_start].strip()
            if not value:
                if self._empty_lines_in_values:
                    # add empty line to the value, but only if there was no
                    # comment on the line
                    if (comment_start is None and
                        cursect is not None and
                        optname and
                        cursect[optname] is not None):
                        cursect[optname].append('') # newlines added at join
                else:
                    # empty line marks end of value
                    indent_level = sys.maxsize
                continue
            # continuation line?
            first_nonspace = self.NONSPACECRE.search(line)
            cur_indent_level = first_nonspace.start() if first_nonspace else 0
            if (cursect is not None and optname and
                cur_indent_level > indent_level):
                cursect[optname].append(value)
            # a section header or option header?
            else:
                indent_level = cur_indent_level
                # is it a section header?
                mo = self.SECTCRE.match(value)
                if mo:
                    sectname = mo.group('header')
                    if sectname in self._sections:
                        if self._strict and sectname in elements_added:
                            raise DuplicateSectionError(sectname, fpname,
                                                        lineno)
                        cursect = self._sections[sectname]
                        elements_added.add(sectname)
                    elif sectname == self.default_section:
                        cursect = self._defaults
                    else:
                        cursect = self._dict()
                        self._sections[sectname] = cursect
                        self._proxies[sectname] = SectionProxy(self, sectname)
                        elements_added.add(sectname)
                    # So sections can't start with a continuation line
                    optname = None
                # no section header in the file?
                elif cursect is None:
                    raise MissingSectionHeaderError(fpname, lineno, line)
                # an option line?
                else:
                    mo = self._optcre.match(value)
                    if mo:
                        optname, vi, optval = mo.group('option', 'vi', 'value')
                        if not optname:
                            e = self._handle_error(e, fpname, lineno, line)
                        optname = self.optionxform(optname.rstrip())
                        if (self._strict and
                            (sectname, optname) in elements_added):
                            raise DuplicateOptionError(sectname, optname,
                                                       fpname, lineno)
                        elements_added.add((sectname, optname))
                        # This check is fine because the OPTCRE cannot
                        # match if it would set optval to None
                        if optval is not None:
                            optval = optval.strip()
                            cursect[optname] = [optval]
                        else:
                            # valueless option handling
                            cursect[optname] = None
                    else:
                        # a non-fatal parsing error occurred. set up the
                        # exception but keep going. the exception will be
                        # raised at the end of the file and will contain a
                        # list of all bogus lines
                        e = self._handle_error(e, fpname, lineno, line)
        # if any parsing errors occurred, raise an exception
        if e:
            raise e
        self._join_multiline_values()

    def _join_multiline_values(self):
        defaults = self.default_section, self._defaults
        all_sections = itertools.chain((defaults,),
                                       self._sections.items())
        for section, options in all_sections:
            for name, val in options.items():
                if isinstance(val, list):
                    val = '\n'.join(val).rstrip()
                options[name] = self._interpolation.before_read(self,
                                                                section,
                                                                name, val)

    def _handle_error(self, exc, fpname, lineno, line):
        if not exc:
            exc = ParsingError(fpname)
        exc.append(lineno, repr(line))
        return exc

    def _unify_values(self, section, vars):
        """
        Creates a sequence of lookups with ``vars`` taking priority over the section which takes priority over the ``DEFAULTSECT``.

        """
        sectiondict = {}
        try:
            sectiondict = self._sections[section]
        except KeyError:
            if section != self.default_section:
                raise NoSectionError(section)
        # Update with the entry specific variables
        vardict = {}
        if vars:
            for key, value in vars.items():
                if value is not None:
                    value = str(value)
                vardict[self.optionxform(key)] = value
        return _ChainMap(vardict, sectiondict, self._defaults)

    def _convert_to_boolean(self, value):
        """
        Returns a boolean value translating from other types if necessary.
        """
        
        if value.lower() not in self.BOOLEAN_STATES:
            raise ValueError('Not a boolean: %s' % value)
        return self.BOOLEAN_STATES[value.lower()]

    def _validate_value_types(self, section="", option="", value=""):
        """Raises a TypeError for non-string values.

        The only legal non-string value if we allow valueless options is None, so we need to check if the value is a string if:
        - we do not allow valueless options, or
        	- we allow valueless options but the value is not None.

        For compatibility reasons this method is not used in classic ``set()`` for *RawConfigParsers*. It is invoked in every case for mapping protocol access and in ``ConfigParser.set()``.
        """
        if not isinstance(section, str):
            raise TypeError("section names must be strings")
        if not isinstance(option, str):
            raise TypeError("option keys must be strings")
        if not self._allow_no_value or value:
            if not isinstance(value, str):
                raise TypeError("option values must be strings")


class ConfigParser(RawConfigParser):
    """
    *ConfigParser* implementing interpolation.
    
    """

    _DEFAULT_INTERPOLATION = BasicInterpolation()

    def set(self, section, option, value=None):
        """
        Set an option.  Extends ``RawConfigParser.set()`` by validating type and interpolation syntax on the value.
        
        """
        self._validate_value_types(option=option, value=value)
        super(ConfigParser, self).set(section, option, value)

    def add_section(self, section):
        """
        Create a new section in the configuration.  Extends ``RawConfigParser.add\_section()`` by validating if the section name is a string.
        
        """
        self._validate_value_types(section=section)
        super(ConfigParser, self).add_section(section)


class SafeConfigParser(ConfigParser):
    """
    *ConfigParser* alias for backwards compatibility purposes.
    
    """

    def __init__(self, *args, **kwargs):
        super(SafeConfigParser, self).__init__(*args, **kwargs)
        warnings.warn(
            "The SafeConfigParser class has been renamed to ConfigParser "
            "in Python 3.2. This alias will be removed in future versions."
            " Use ConfigParser directly instead.",
            DeprecationWarning, stacklevel=2
        )


class SectionProxy(MutableMapping):
    """
    A proxy for a single section from a parser.
    
    """

    def __init__(self, parser, name):
        """Creates a view on a section of the specified `name` in `parser`."""
        self._parser = parser
        self._name = name

    def __repr__(self):
        return '<Section: {}>'.format(self._name)

    def __getitem__(self, key):
        if not self._parser.has_option(self._name, key):
            raise KeyError(key)
        return self._parser.get(self._name, key)

    def __setitem__(self, key, value):
        self._parser._validate_value_types(option=key, value=value)
        return self._parser.set(self._name, key, value)

    def __delitem__(self, key):
        if not (self._parser.has_option(self._name, key) and
                self._parser.remove_option(self._name, key)):
            raise KeyError(key)

    def __contains__(self, key):
        return self._parser.has_option(self._name, key)

    def __len__(self):
        return len(self._options())

    def __iter__(self):
        return self._options().__iter__()

    def _options(self):
        if self._name != self._parser.default_section:
            return self._parser.options(self._name)
        else:
            return self._parser.defaults()

    def get(self, option, fallback=None, raw=False, vars=None):
        return self._parser.get(self._name, option, raw=raw, vars=vars,
                                fallback=fallback)

    def getint(self, option, fallback=None, raw=False, vars=None):
        return self._parser.getint(self._name, option, raw=raw, vars=vars,
                                   fallback=fallback)

    def getfloat(self, option, fallback=None, raw=False, vars=None):
        return self._parser.getfloat(self._name, option, raw=raw, vars=vars,
                                     fallback=fallback)

    def getboolean(self, option, fallback=None, raw=False, vars=None):
        return self._parser.getboolean(self._name, option, raw=raw, vars=vars,
                                       fallback=fallback)

    @property
    def parser(self):
        # The parser object of the proxy is read-only.
        return self._parser

    @property
    def name(self):
        # The name of the section on a proxy is read-only.
        return self._name


class _ExtendedConfigParser(RawConfigParser):
	"""
	*ConfigParser* with enhancements.
	
	"""

	_DEFAULT_INTERPOLATION = ExtendedInterpolation()
	# Compiled regular expression for matching options with type information
	_OPTYRE = re.compile(r"^(?P<option>\S+?)@@(?P<type>[a-zA-Z0-9&_\-\.]+)$")

	def optionxform(self, optionstr):
		"""
		Replacement of the base one to retrieve the type of option.
		
		"""
		if self.reading: # Because when it reads it needs to know the type while it writes it must send as is.
			result = self._OPTYRE.match(optionstr)
			if result is not None:
				self.optionMap[result.group('option')] = result.group('type')
				return result.group('option')
		return optionstr

	def get(self, section, option, raw = False):
		"""
		Get an option.
		
		:Parameters:
			- *section* (float|integer|boolean): TODO. 
			- *option* (string): Option name.
			- *raw* (boolean): Is raw?.
		
		:Returns: The value of option.
		
		"""
		self.reading = False
		if section is None:
			section = self.default_section
		else:
			if isinstance(section, int) or isinstance(section, long) or isinstance(section, float) or isinstance(section, bool):
				section = str(section)
			elif isinstance(section, bytes) and not isPrintable(section):
				section = str(binascii.hexlify(section))
			self._validate_value_types(section = section)
			if not super(_ExtendedConfigParser, self).has_section(section):
				error._lay('Section not Found: ' + section)
				return None
		value = super(_ExtendedConfigParser, self).get(section, option, fallback = None)
		if value is not None and option in self.optionMap:
			value_type = self.optionMap[option]
			if value_type == 'float':
				return float(value)
			elif value_type == 'int':
				return int(value)
			elif value_type == 'long':
				return long(value)
			elif value_type == 'bool':
				return self._convert_to_boolean(value)
			elif value_type == 'bytes':
				return binascii.unhexlify(value)
			elif value_type == 'str':
				return str(binascii.unhexlify(value))
			elif value_type == 'list':
				dataString = value.split('\t')
				dataList = []
				for data in dataString:
					if data.startswith('float@'):
						dataList.append(float(data[6:]))
					elif data.startswith('int@'):
						dataList.append(int(data[4:]))
					elif data.startswith('long@'):
						dataList.append(long(data[5:]))
					elif data.startswith('bool@'):
						dataList.append(self._convert_to_boolean(data[5:]))
					elif data.startswith('bytes@'):
						dataList.append(binascii.unhexlify(data[6:]))
					elif data.startswith('str@'):
						dataList.append(str(binascii.unhexlify(data[4:])))
					else:
						dataList.append(data)
				return dataList
			else:
				result = re.match('^(?P<module>[a-zA-Z0-9_\.]+)&(?P<function>\w+)$', value_type)
				if result is not None:
					module = result.group('module')
					obj = __import__ (module, fromlist = [ module[module.rfind('.')+1:] ])
					func = getattr(obj, result.group('function'), None)
					if callable(func):
						try:
							return func(*value.split())
						except:
							error._lay('Failed to call the function [' + option + ']: ' + sys.exc_info()[0])
							return None
				error._lay('Failed to recognize the type: ' + option)
				return None
		return value

	def set(self, section, option, value = None):
		"""
		Set an option.  Extends *RawConfigParser.set()* by validating type and interpolation syntax on the value.
		
		:Parameters:
			- *section* (float|integer|boolean): TODO. 
			- *option* (string): Option name.
			- *value* (string): Option raw.
		
		"""
		if section is not None:
			section = self.add_section(section)
		else:
			self.reading = True
		self._validate_value_types(option = option, value = value)
		self.modified = True
		super(_ExtendedConfigParser, self).set(section, option, value)

	def add_section(self, section):
		"""
		Create a new section in the configuration. Extends ``RawConfigParser.add\_section()`` by validating if the section name is a string.
		
		"""
		self.reading = True
		if isinstance(section, int) or isinstance(section, long) or isinstance(section, float) or isinstance(section, bool):
			section = str(section)
		elif isinstance(section, bytes) and not isPrintable(section):
			section = str(binascii.hexlify(section))
		self._validate_value_types(section = section)
		if not super(_ExtendedConfigParser, self).has_section(section):
			super(_ExtendedConfigParser, self).add_section(section)
		return section

	def read(self, fileINI):
		"""
		Sets the *INI file* to read/write and read It.
		
		:Parameters:
			- *fileINI* (string): INI file name.
		
		"""
		self.BOOLEAN_STATES = {'1': True, 'yes': True, 'true': True, 'on': True, 'sure': True, 'accept': True, 'enabled': True,
	                           '0': False, 'no': False, 'false': False, 'off': False, 'nope': False, 'reject': False, 'disabled': False}
		self.iniFile = fileINI
		self.reading = True
		self.modified = False
		self.optionMap = {}
		super(_ExtendedConfigParser, self).read(fileINI)
		self.setBuiltIn(True)

	def _write_section(self, fp, section_name, section_items, delimiter):
		""""
		Helper needed to rearrange option adding type on writing.
		
		:Parameters:
			- *fp* (string): file pointer.
			- *section\_name* (string): Section name.
			- *section\_items* (list): Items list.
			- *delimiter* (string): Delimiter between sections.
			
		
		"""
		remanaged_items = [ (key if key not in self.optionMap else key + '@@' + self.optionMap[key], value) for key, value in section_items ]
		super(_ExtendedConfigParser, self)._write_section(fp, section_name, remanaged_items, delimiter)

	def write(self, end = True):
		"""
		Writes changes to the *INI file*.
		
		:Parameters:
			- *end* (boolean): If ``True`` the *Built-in* flag is enabled.
		
		"""
		if self.modified:
			self.reading = False
			tmpConfig = None if not os.path.exists(self.iniFile) else self.iniFile + '_' + str(os.getpid())
			try:
				if tmpConfig is not None:
					os.rename(self.iniFile, tmpConfig)
			except:
				error._lay('Failed to rename [' + os.path.basename(self.iniFile) + ']: ' + str(sys.exc_info()[1]))
			else:
				restoreConfig = True
				try:
					configfile = open(self.iniFile, 'w')
				except:
					error._lay('Failed to open [' + os.path.basename(self.iniFile) + ']: ' + str(sys.exc_info()[1]))
				else:
					try:
						self.setBuiltIn(False)
						super(_ExtendedConfigParser, self).write(configfile)
						if not end:
							self.setBuiltIn(True)
						restoreConfig = False
					except:
						error._lay('Failed to write [' + os.path.basename(self.iniFile) + ']: ' + str(sys.exc_info()[1]))
					finally:
						configfile.close()
					if tmpConfig is not None:
						if restoreConfig:
							try:
								os.remove(self.iniFile)
								os.rename(tmpConfig, self.iniFile)
							except:
								error._lay('Failed to restore [' + os.path.basename(self.iniFile) + ']: ' + str(sys.exc_info()[1]))
						else:
							try:
								os.remove(tmpConfig)
							except:
								error._lay('Failed to remove [' + os.path.basename(tmpConfig) + ']: ' + str(sys.exc_info()[1]))

	def remove(self, section, option):
		"""
		Remove an option and eventually the related section if empty.
		
		:Parameters:
			- *section* (string): Section name.
			- *option* (string): Option name.
		
		:Returns: ``True`` if option exist, ``False`` otherwise. 
		
		"""
		self.reading = False
		existed = super(_ExtendedConfigParser, self).remove_option(section, option)
		if existed and section is not None and section != self.default_section:
			sectdict = self._sections[section]
			if len(sectdict) == 0:
				super(_ExtendedConfigParser, self).remove_section(section)
		return existed

	def setBuiltIn(self, inside):
		"""
		Built\-in section for option set in the codes.
		
		:Parameters:
			- *inside* (boolean): Is a Built\-in section?.
			
		"""
		if inside:
			super(_ExtendedConfigParser, self).add_section('__BUILTIN__')
			super(_ExtendedConfigParser, self).set('__BUILTIN__', '__DIR__', os.path.dirname(self.iniFile))
		else:
			super(_ExtendedConfigParser, self).remove_section('__BUILTIN__')

def isPrintable(theString):
	"""
	Checks if a string is printable.
		
		:Parameters:
			- *theString* (string): String to check.
			
	:Returns: ``True`` if the string is printable, ``False`` otherwise.
			
	"""
	import string
	printset = set(string.printable)
	return set(theString).issubset(printset)

class DataStore(object):
	"""
	This class handles a data store.
	  
	"""

	def __init__(self, path = None, dirname = 'data', configname = 'config.ini', default_section = 'common', throw = False, readonly = False):
		"""
		Initializes a *DataStore* object.
		 
		:Parameters:
			- *path* (string): Path to data store directory.
			- *dirname* (string): Data store directory. 
			- *configname* (string): INI file name.
			- *default\_section* (string): Default section name.
			- *throw* (boolean): Throws exceptions?.  
			- *readonly* (boolean): ``True`` if the data store is read\-only, ``False`` if it's writable.  
		 
		"""
		# self.dirname = os.path.dirname(__file__)
		self.datapath = os.path.join(os.curdir if path is None else path, dirname)
		if not os.path.exists(self.datapath):
			os.makedirs(self.datapath, mode = 0777)
		self.throw = throw
		self.readonly = readonly
		self.inifile = _ExtendedConfigParser(default_section = default_section, strict = True, allow_no_value = True)
		self.inifile.read(os.path.join(self.datapath, configname))
		self.RSA_FORMAT = self.inifile.get(None, 'FormatOfRSA')
		if self.RSA_FORMAT is None:
			self.RSA_FORMAT = 'PEM'

	def __del__(self):
		if not self.readonly:
			self.inifile.write()

	def sync(self, forcewrite = False):
		"""
		If the INI file isn't read-only, writes changes.
		
		:Parameters:
			- *forcewrite* (boolean): Force the write.
		
		"""
		if forcewrite or not self.readonly:
			self.inifile.write(False)

	def put(self, name, data, section = None):
		"""
		Puts a new item to the *Data store*.
		
		:Parameters:
			- *name* (string): Item name.
			- *data* (int|float|boolean): Item to put in the Data store.
			- *section* (string): Section name.
		
		"""
		if isinstance(data, int) or isinstance(data, long) or isinstance(data, float) or isinstance(data, bool):
			self.inifile.set(section, name + '@@' + data.__class__.__name__, str(data))
		elif isinstance(data, list):
			dataString = None
			for d in data:
				s = None
				if isinstance(d, int) or isinstance(d, long) or isinstance(d, float) or isinstance(d, bool):
					s = d.__class__.__name__ + '@' + str(d)
				elif isinstance(d, str):
					if isPrintable(d): s = d
					else: s = d.__class__.__name__ + '@' + str(binascii.hexlify(d))
				if s is not None:
					if dataString is None: dataString = s
					else: dataString += '\t' + s
			self.inifile.set(section, name + '@@' + data.__class__.__name__, dataString)
		elif isinstance(data, str):
			if isPrintable(data):
				self.inifile.set(section, name, data)
			else:
				self.inifile.set(section, name + '@@' + data.__class__.__name__, str(binascii.hexlify(data)))
		else:
			if section is None:
				file_ext = ''
			elif isinstance(section, str):
				file_ext = section
			elif isinstance(section, int) or isinstance(section, long) or isinstance(section, float) or isinstance(section, bool):
				file_ext = str(section)
			else:
				error._lay('[ ' + name + ' ] Cannot convert its section: ' + data.__class__.__name__)
				if self.throw:
					raise Error
				else:
					return False
			try:
				className = data.__class__.__name__
				if className == "_RSAobj":
					dataToStore = data.exportKey(format = self.RSA_FORMAT)
					file_ext += '.cryRSA.' + self.RSA_FORMAT
				elif className == "PublicKey" or className == "PrivateKey":
					dataToStore = data.save_pkcs1(format = self.RSA_FORMAT)
					file_ext += '.pyRSA.' + self.RSA_FORMAT
				else:
					error._lay('[ ' + name + ' ] Unknown class: ' + className)
					if self.throw:
						raise Error
					else:
						return False
				filepath = os.path.join(self.datapath, name + file_ext)
				with open(filepath, 'wb') as file:
					file.write(dataToStore)
				file.close()
			except TypeError:
				error._lay('[ ' + name + ' ] Unknown type: ' + data.__class__)
				if self.throw:
					raise Error
				else:
					return False
			except Error:
				raise
			except:
				error._lay('Unexpected storing error: ' + sys.exc_info()[0])
				if self.throw:
					raise Error
				else:
					return False
			return True
		return True

	def get(self, name, section = None, remove = False):
		"""
		Gets an item from the *Data store*.
		
		:Parameters:
			- *name* (string): Item name.
			- *section* (string): Section name.
			- *remove* (boolean): Remove the item from the *Data store*?
		
		"""
		data = self.inifile.get(section, name)
		if data is None:
			# Search for stored data file
			if section is None: dataRE = re.compile('^' + name + "\.(?P<type>\w+)\.(?P<ext>\w+)$")
			else: dataRE = re.compile('^' + name + "\.(?P<sect>\w+?)\.(?P<type>\w+?)\.(?P<ext>\w+)$")
			for file in os.listdir(self.datapath):
				fileMatched = dataRE.match(file)
				if fileMatched is not None:
					filepath = os.path.join(self.datapath, file)
					dataType = fileMatched.group('type')
					dataExt = fileMatched.group('ext')
					if section is None or section == fileMatched.group('sect'):
						break
			else:
				error._lay('NOT Found as stored: ' + name)
				if self.throw:
					raise Error
				else:
					return None
			with open(filepath, 'rb') as file:
				try:
					dataRead = file.read()
					if dataType == 'bytes':
						data = dataRead
					elif dataType == 'cryRSA':
						from Crypto.PublicKey import RSA
						data = RSA.importKey(dataRead)
					elif dataType == 'pyRSA':
						import rsa as RSA
						try:
							data = RSA.PrivateKey.load_pkcs1(dataRead, format = dataExt)
						except:
							data = RSA.PublicKey.load_pkcs1(dataRead, format = dataExt)
					else:
						error._lay('Unknown type of stored data: ' + dataType)
						if self.throw:
							raise Error
						else:
							return None
					if remove:
						try:
							os.remove(filepath)
						except:
							error._lay('Failed to remove < ' + name + ' >: ' + str(sys.exc_info()[1]))
							if self.throw:
								raise Error
					return data
				except Error:
					raise
				except:
					error._lay('Unexpected restoring error: ' + str(sys.exc_info()[1]))
					if self.throw:
						raise Error
					else:
						return None
			file.close()
		else:
			if remove:
				self.inifile.remove(section, name)
			return data

	def remove(self, name, section = None):
		"""
		Removes an item from the *Data store*.
		
		:Parameters:
			- *name* (string): Item name.
			- *section* (string): Section name.
		
		"""
		if not self.inifile.remove(section, name):
			# Search for stored data file
			if section is None: dataRE = re.compile('^' + name + "\.(?P<type>\w+)\.(?P<ext>\w+)$")
			else: dataRE = re.compile('^' + name + "\.(?P<sect>\w+?)\.(?P<type>\w+?)\.(?P<ext>\w+)$")
			for file in os.listdir(self.datapath):
				fileMatched = dataRE.match(file)
				if fileMatched is not None:
					filepath = os.path.join(self.datapath, file)
					try:
						os.remove(filepath)
					except:
						error._lay('Failed to remove < ' + name + ' >: ' + str(sys.exc_info()[1]))
						if self.throw:
							raise Error
						else:
							return
					break
			else:
				error._lay('NOT Found as stored: ' + name)
				if self.throw:
					raise Error
