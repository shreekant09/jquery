#line 1 "ext/hpricot_scan/hpricot_scan.rl"
/*
 * hpricot_scan.rl
 *
 * $Author: why $
 * $Date: 2006-05-08 22:03:50 -0600 (Mon, 08 May 2006) $
 *
 * Copyright (C) 2006 why the lucky stiff
 */
#include <ruby.h>

#define NO_WAY_SERIOUSLY "*** This should not happen, please send a bug report with the HTML you're parsing to why@whytheluckystiff.net.  So sorry!"

static VALUE sym_xmldecl, sym_doctype, sym_procins, sym_stag, sym_etag, sym_emptytag, sym_comment,
      sym_cdata, sym_text;
static VALUE rb_eHpricotParseError;
static ID s_read, s_to_str;

#define ELE(N) \
  if (tokend > tokstart || text == 1) { \
    VALUE raw_string = Qnil; \
    ele_open = 0; text = 0; \
    if (tokstart != 0 && sym_##N != sym_cdata && sym_##N != sym_text && sym_##N != sym_procins && sym_##N != sym_comment) { \
      raw_string = rb_str_new(tokstart, tokend-tokstart); \
    } \
    rb_yield_tokens(sym_##N, tag, attr, raw_string, taint); \
  }

#define SET(N, E) \
  if (mark_##N == NULL || E == mark_##N) \
    N = rb_str_new2(""); \
  else if (E > mark_##N) \
    N = rb_str_new(mark_##N, E - mark_##N);

#define CAT(N, E) if (NIL_P(N)) { SET(N, E); } else { rb_str_cat(N, mark_##N, E - mark_##N); }

#define SLIDE(N) if ( mark_##N > tokstart ) mark_##N = buf + (mark_##N - tokstart);

#define ATTR(K, V) \
    if (!NIL_P(K)) { \
      if (NIL_P(attr)) attr = rb_hash_new(); \
      rb_hash_aset(attr, K, V); \
    }

#define TEXT_PASS() \
    if (text == 0) \
    { \
      if (ele_open == 1) { \
        ele_open = 0; \
        if (tokstart > 0) { \
          mark_tag = tokstart; \
        } \
      } else { \
        mark_tag = p; \
      } \
      attr = Qnil; \
      tag = Qnil; \
      text = 1; \
    }

#define EBLK(N, T) CAT(tag, p - T + 1); ELE(N);

#line 107 "ext/hpricot_scan/hpricot_scan.rl"



#line 68 "ext/hpricot_scan/hpricot_scan.c"
static const int hpricot_scan_start = 204;
static const int hpricot_scan_error = -1;

static const int hpricot_scan_en_html_comment = 214;
static const int hpricot_scan_en_html_cdata = 216;
static const int hpricot_scan_en_html_procins = 218;
static const int hpricot_scan_en_main = 204;

#line 110 "ext/hpricot_scan/hpricot_scan.rl"

#define BUFSIZE 16384

void rb_yield_tokens(VALUE sym, VALUE tag, VALUE attr, VALUE raw, int taint)
{
  VALUE ary;
  if (sym == sym_text) {
    raw = tag;
  }
  ary = rb_ary_new3(4, sym, tag, attr, raw);
  if (taint) { 
    OBJ_TAINT(ary);
    OBJ_TAINT(tag);
    OBJ_TAINT(attr);
    OBJ_TAINT(raw);
  }
  rb_yield(ary);
}

VALUE hpricot_scan(VALUE self, VALUE port)
{
  int cs, act, have = 0, nread = 0, curline = 1, text = 0;
  char *tokstart = 0, *tokend = 0, *buf = NULL;

  VALUE attr = Qnil, tag = Qnil, akey = Qnil, aval = Qnil, bufsize = Qnil;
  char *mark_tag = 0, *mark_akey = 0, *mark_aval = 0;
  int done = 0, ele_open = 0, buffer_size = 0;

  int taint = OBJ_TAINTED( port );
  if ( !rb_respond_to( port, s_read ) )
  {
    if ( rb_respond_to( port, s_to_str ) )
    {
      port = rb_funcall( port, s_to_str, 0 );
      StringValue(port);
    }
    else
    {
      rb_raise( rb_eArgError, "bad Hpricot argument, String or IO only please." );
    }
  }

  buffer_size = BUFSIZE;
  if (rb_ivar_defined(self, rb_intern("@buffer_size")) == Qtrue) {
    bufsize = rb_ivar_get(self, rb_intern("@buffer_size"));
    if (!NIL_P(bufsize)) {
      buffer_size = NUM2INT(bufsize);
    }
  }
  buf = ALLOC_N(char, buffer_size);

  
#line 130 "ext/hpricot_scan/hpricot_scan.c"
	{
	cs = hpricot_scan_start;
	tokstart = 0;
	tokend = 0;
	act = 0;
	}
#line 162 "ext/hpricot_scan/hpricot_scan.rl"
  
  while ( !done ) {
    VALUE str;
    char *p = buf + have, *pe;
    int len, space = buffer_size - have;

    if ( space == 0 ) {
      /* We've used up the entire buffer storing an already-parsed token
       * prefix that must be preserved.  Likely caused by super-long attributes.
       * See ticket #13. */
      rb_raise(rb_eHpricotParseError, "ran out of buffer space on element <%s>, starting on line %d.", RSTRING(tag)->ptr, curline);
    }

    if ( rb_respond_to( port, s_read ) )
    {
      str = rb_funcall( port, s_read, 1, INT2FIX(space) );
    }
    else
    {
      str = rb_str_substr( port, nread, space );
    }

    StringValue(str);
    memcpy( p, RSTRING(str)->ptr, RSTRING(str)->len );
    len = RSTRING(str)->len;
    nread += len;

    /* If this is the last buffer, tack on an EOF. */
    if ( len < space ) {
      p[len++] = 0;
      done = 1;
    }

    pe = p + len;
    
#line 173 "ext/hpricot_scan/hpricot_scan.c"
	{
	if ( p == pe )
		goto _out;
	switch ( cs )
	{
tr0:
#line 73 "ext/hpricot_scan/hpricot_scan.rl"
	{{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st204;
tr4:
#line 71 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ {{p = ((tokend))-1;}{goto st214;}} }{p = ((tokend))-1;}}
	goto st204;
tr15:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(doctype); }{p = ((tokend))-1;}}
	goto st204;
tr18:
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(doctype); }{p = ((tokend))-1;}}
	goto st204;
tr39:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{	switch( act ) {
	case 8:
	{ ELE(doctype); }
	break;
	case 10:
	{ ELE(stag); }
	break;
	case 12:
	{ ELE(emptytag); }
	break;
	case 15:
	{ TEXT_PASS(); }
	break;
	default: break;
	}
	{p = ((tokend))-1;}}
	goto st204;
tr89:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ {{p = ((tokend))-1;}{goto st216;}} }{p = ((tokend))-1;}}
	goto st204;
tr93:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 69 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(etag); }{p = ((tokend))-1;}}
	goto st204;
tr95:
#line 69 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(etag); }{p = ((tokend))-1;}}
	goto st204;
tr99:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr103:
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr108:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr113:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr114:
#line 70 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(emptytag); }{p = ((tokend))-1;}}
	goto st204;
tr125:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr129:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr135:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }{p = ((tokend))-1;}}
	goto st204;
tr341:
#line 67 "ext/hpricot_scan/hpricot_scan.rl"
	{{ {{p = ((tokend))-1;}{goto st218;}} }{p = ((tokend))-1;}}
	goto st204;
tr358:
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(xmldecl); }{p = ((tokend))-1;}}
	goto st204;
tr403:
#line 73 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st204;
tr404:
#line 9 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
#line 73 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st204;
tr406:
#line 73 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st204;
tr411:
#line 67 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ {{p = ((tokend))-1;}{goto st218;}} }{p = ((tokend))-1;}}
	goto st204;
st204:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out204;
case 204:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = p;}
#line 337 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr404;
		case 60: goto tr405;
	}
	goto tr403;
tr405:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{
    if (text == 1) {
      CAT(tag, p);
      ELE(text);
      text = 0;
    }
    attr = Qnil;
    tag = Qnil;
    mark_tag = NULL;
    ele_open = 1;
  }
#line 73 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 15;}
	goto st205;
st205:
	if ( ++p == pe )
		goto _out205;
case 205:
#line 365 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 33: goto st0;
		case 47: goto st59;
		case 58: goto tr409;
		case 63: goto st145;
		case 95: goto tr409;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr409;
	} else if ( (*p) >= 65 )
		goto tr409;
	goto tr406;
st0:
	if ( ++p == pe )
		goto _out0;
case 0:
	switch( (*p) ) {
		case 45: goto st1;
		case 68: goto st2;
		case 91: goto st53;
	}
	goto tr0;
st1:
	if ( ++p == pe )
		goto _out1;
case 1:
	if ( (*p) == 45 )
		goto tr4;
	goto tr0;
st2:
	if ( ++p == pe )
		goto _out2;
case 2:
	if ( (*p) == 79 )
		goto st3;
	goto tr0;
st3:
	if ( ++p == pe )
		goto _out3;
case 3:
	if ( (*p) == 67 )
		goto st4;
	goto tr0;
st4:
	if ( ++p == pe )
		goto _out4;
case 4:
	if ( (*p) == 84 )
		goto st5;
	goto tr0;
st5:
	if ( ++p == pe )
		goto _out5;
case 5:
	if ( (*p) == 89 )
		goto st6;
	goto tr0;
st6:
	if ( ++p == pe )
		goto _out6;
case 6:
	if ( (*p) == 80 )
		goto st7;
	goto tr0;
st7:
	if ( ++p == pe )
		goto _out7;
case 7:
	if ( (*p) == 69 )
		goto st8;
	goto tr0;
st8:
	if ( ++p == pe )
		goto _out8;
case 8:
	if ( (*p) == 32 )
		goto st9;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st9;
	goto tr0;
st9:
	if ( ++p == pe )
		goto _out9;
case 9:
	switch( (*p) ) {
		case 32: goto st9;
		case 58: goto tr12;
		case 95: goto tr12;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st9;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr12;
	} else
		goto tr12;
	goto tr0;
tr12:
#line 77 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _out10;
case 10:
#line 473 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr13;
		case 62: goto tr15;
		case 63: goto st10;
		case 91: goto tr16;
		case 95: goto st10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st10;
		} else if ( (*p) >= 9 )
			goto tr13;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st10;
		} else if ( (*p) >= 65 )
			goto st10;
	} else
		goto st10;
	goto tr0;
tr13:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _out11;
case 11:
#line 504 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st11;
		case 62: goto tr18;
		case 80: goto st12;
		case 83: goto st48;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st11;
	goto tr0;
st12:
	if ( ++p == pe )
		goto _out12;
case 12:
	if ( (*p) == 85 )
		goto st13;
	goto tr0;
st13:
	if ( ++p == pe )
		goto _out13;
case 13:
	if ( (*p) == 66 )
		goto st14;
	goto tr0;
st14:
	if ( ++p == pe )
		goto _out14;
case 14:
	if ( (*p) == 76 )
		goto st15;
	goto tr0;
st15:
	if ( ++p == pe )
		goto _out15;
case 15:
	if ( (*p) == 73 )
		goto st16;
	goto tr0;
st16:
	if ( ++p == pe )
		goto _out16;
case 16:
	if ( (*p) == 67 )
		goto st17;
	goto tr0;
st17:
	if ( ++p == pe )
		goto _out17;
case 17:
	if ( (*p) == 32 )
		goto st18;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto tr0;
st18:
	if ( ++p == pe )
		goto _out18;
case 18:
	switch( (*p) ) {
		case 32: goto st18;
		case 34: goto st19;
		case 39: goto st30;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto tr0;
st19:
	if ( ++p == pe )
		goto _out19;
case 19:
	switch( (*p) ) {
		case 9: goto tr30;
		case 34: goto tr31;
		case 61: goto tr30;
		case 95: goto tr30;
	}
	if ( (*p) < 39 ) {
		if ( 32 <= (*p) && (*p) <= 37 )
			goto tr30;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr30;
		} else if ( (*p) >= 63 )
			goto tr30;
	} else
		goto tr30;
	goto tr0;
tr30:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st20;
st20:
	if ( ++p == pe )
		goto _out20;
case 20:
#line 601 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st20;
		case 34: goto tr31;
		case 61: goto st20;
		case 95: goto st20;
	}
	if ( (*p) < 39 ) {
		if ( 32 <= (*p) && (*p) <= 37 )
			goto st20;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st20;
		} else if ( (*p) >= 63 )
			goto st20;
	} else
		goto st20;
	goto tr0;
tr31:
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _out21;
case 21:
#line 628 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st22;
		case 62: goto tr18;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st22;
	goto tr0;
st22:
	if ( ++p == pe )
		goto _out22;
case 22:
	switch( (*p) ) {
		case 32: goto st22;
		case 34: goto st23;
		case 39: goto st28;
		case 62: goto tr18;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st22;
	goto tr0;
st23:
	if ( ++p == pe )
		goto _out23;
case 23:
	if ( (*p) == 34 )
		goto tr37;
	goto tr36;
tr36:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _out24;
case 24:
#line 666 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr37;
	goto st24;
tr37:
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _out25;
case 25:
#line 678 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st25;
		case 62: goto tr18;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st25;
	goto tr39;
tr16:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _out26;
case 26:
#line 695 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 93 )
		goto st27;
	goto st26;
st27:
	if ( ++p == pe )
		goto _out27;
case 27:
	switch( (*p) ) {
		case 32: goto st27;
		case 62: goto tr18;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st27;
	goto tr39;
st28:
	if ( ++p == pe )
		goto _out28;
case 28:
	if ( (*p) == 39 )
		goto tr37;
	goto tr42;
tr42:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st29;
st29:
	if ( ++p == pe )
		goto _out29;
case 29:
#line 725 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr37;
	goto st29;
st30:
	if ( ++p == pe )
		goto _out30;
case 30:
	switch( (*p) ) {
		case 9: goto tr44;
		case 39: goto tr45;
		case 61: goto tr44;
		case 95: goto tr44;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto tr44;
		} else if ( (*p) >= 32 )
			goto tr44;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr44;
		} else if ( (*p) >= 63 )
			goto tr44;
	} else
		goto tr44;
	goto tr0;
tr44:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st31;
st31:
	if ( ++p == pe )
		goto _out31;
case 31:
#line 762 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st31;
		case 39: goto tr47;
		case 61: goto st31;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 32 )
			goto st31;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 63 )
			goto st31;
	} else
		goto st31;
	goto tr0;
tr47:
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st32;
tr45:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st32;
tr53:
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _out32;
case 32:
#line 804 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st33;
		case 32: goto st33;
		case 33: goto st31;
		case 39: goto tr47;
		case 62: goto tr18;
		case 91: goto st26;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 10 )
			goto st22;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 61 )
			goto st31;
	} else
		goto st31;
	goto tr0;
st33:
	if ( ++p == pe )
		goto _out33;
case 33:
	switch( (*p) ) {
		case 9: goto st33;
		case 32: goto st33;
		case 34: goto st23;
		case 39: goto tr49;
		case 62: goto tr18;
		case 91: goto st26;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 10 )
			goto st22;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 61 )
			goto st31;
	} else
		goto st31;
	goto tr0;
tr49:
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st34;
tr60:
#line 91 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st34;
st34:
	if ( ++p == pe )
		goto _out34;
case 34:
#line 871 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto tr50;
		case 32: goto tr50;
		case 33: goto tr52;
		case 39: goto tr53;
		case 62: goto tr54;
		case 91: goto tr55;
		case 95: goto tr52;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto tr52;
		} else if ( (*p) >= 10 )
			goto tr51;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr52;
		} else if ( (*p) >= 61 )
			goto tr52;
	} else
		goto tr52;
	goto tr42;
tr50:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st35;
st35:
	if ( ++p == pe )
		goto _out35;
case 35:
#line 904 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st35;
		case 32: goto st35;
		case 34: goto st37;
		case 39: goto tr60;
		case 62: goto tr61;
		case 91: goto st40;
		case 95: goto st47;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 37 )
				goto st47;
		} else if ( (*p) >= 10 )
			goto st36;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else if ( (*p) >= 61 )
			goto st47;
	} else
		goto st47;
	goto st29;
tr51:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st36;
st36:
	if ( ++p == pe )
		goto _out36;
case 36:
#line 937 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st36;
		case 34: goto st37;
		case 39: goto tr63;
		case 62: goto tr61;
		case 91: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st36;
	goto st29;
st37:
	if ( ++p == pe )
		goto _out37;
case 37:
	switch( (*p) ) {
		case 34: goto tr65;
		case 39: goto tr66;
	}
	goto tr64;
tr64:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _out38;
case 38:
#line 965 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr65;
		case 39: goto tr68;
	}
	goto st38;
tr78:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st39;
tr65:
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _out39;
case 39:
#line 983 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st39;
		case 39: goto tr37;
		case 62: goto tr61;
		case 91: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st39;
	goto st29;
tr54:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 8;}
	goto st206;
tr61:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 8;}
	goto st206;
st206:
	if ( ++p == pe )
		goto _out206;
case 206:
#line 1011 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr37;
	goto st29;
tr55:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st40;
st40:
	if ( ++p == pe )
		goto _out40;
case 40:
#line 1023 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr70;
		case 93: goto st42;
	}
	goto st40;
tr70:
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _out41;
case 41:
#line 1037 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st41;
		case 62: goto tr73;
		case 93: goto st27;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st41;
	goto st26;
tr73:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 8;}
	goto st207;
st207:
	if ( ++p == pe )
		goto _out207;
case 207:
#line 1056 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 93 )
		goto st27;
	goto st26;
st42:
	if ( ++p == pe )
		goto _out42;
case 42:
	switch( (*p) ) {
		case 32: goto st42;
		case 39: goto tr37;
		case 62: goto tr61;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st42;
	goto st29;
tr68:
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st43;
tr66:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _out43;
case 43:
#line 1086 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st43;
		case 34: goto tr37;
		case 62: goto tr75;
		case 91: goto st44;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st43;
	goto st24;
tr75:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 66 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 8;}
	goto st208;
st208:
	if ( ++p == pe )
		goto _out208;
case 208:
#line 1106 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr37;
	goto st24;
st44:
	if ( ++p == pe )
		goto _out44;
case 44:
	switch( (*p) ) {
		case 34: goto tr70;
		case 93: goto st45;
	}
	goto st44;
st45:
	if ( ++p == pe )
		goto _out45;
case 45:
	switch( (*p) ) {
		case 32: goto st45;
		case 34: goto tr37;
		case 62: goto tr75;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st45;
	goto st24;
tr63:
#line 92 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st46;
st46:
	if ( ++p == pe )
		goto _out46;
case 46:
#line 1139 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr78;
		case 39: goto tr37;
		case 62: goto tr54;
		case 91: goto tr55;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr78;
	goto tr42;
tr52:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st47;
st47:
	if ( ++p == pe )
		goto _out47;
case 47:
#line 1157 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st47;
		case 39: goto tr53;
		case 61: goto st47;
		case 95: goto st47;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st47;
		} else if ( (*p) >= 32 )
			goto st47;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else if ( (*p) >= 63 )
			goto st47;
	} else
		goto st47;
	goto st29;
st48:
	if ( ++p == pe )
		goto _out48;
case 48:
	if ( (*p) == 89 )
		goto st49;
	goto tr0;
st49:
	if ( ++p == pe )
		goto _out49;
case 49:
	if ( (*p) == 83 )
		goto st50;
	goto tr0;
st50:
	if ( ++p == pe )
		goto _out50;
case 50:
	if ( (*p) == 84 )
		goto st51;
	goto tr0;
st51:
	if ( ++p == pe )
		goto _out51;
case 51:
	if ( (*p) == 69 )
		goto st52;
	goto tr0;
st52:
	if ( ++p == pe )
		goto _out52;
case 52:
	if ( (*p) == 77 )
		goto st21;
	goto tr0;
st53:
	if ( ++p == pe )
		goto _out53;
case 53:
	if ( (*p) == 67 )
		goto st54;
	goto tr0;
st54:
	if ( ++p == pe )
		goto _out54;
case 54:
	if ( (*p) == 68 )
		goto st55;
	goto tr0;
st55:
	if ( ++p == pe )
		goto _out55;
case 55:
	if ( (*p) == 65 )
		goto st56;
	goto tr0;
st56:
	if ( ++p == pe )
		goto _out56;
case 56:
	if ( (*p) == 84 )
		goto st57;
	goto tr0;
st57:
	if ( ++p == pe )
		goto _out57;
case 57:
	if ( (*p) == 65 )
		goto st58;
	goto tr0;
st58:
	if ( ++p == pe )
		goto _out58;
case 58:
	if ( (*p) == 91 )
		goto tr89;
	goto tr0;
st59:
	if ( ++p == pe )
		goto _out59;
case 59:
	switch( (*p) ) {
		case 58: goto tr90;
		case 95: goto tr90;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr90;
	} else if ( (*p) >= 65 )
		goto tr90;
	goto tr0;
tr90:
#line 77 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st60;
st60:
	if ( ++p == pe )
		goto _out60;
case 60:
#line 1278 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr91;
		case 62: goto tr93;
		case 63: goto st60;
		case 95: goto st60;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st60;
		} else if ( (*p) >= 9 )
			goto tr91;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st60;
		} else if ( (*p) >= 65 )
			goto st60;
	} else
		goto st60;
	goto tr0;
tr91:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _out61;
case 61:
#line 1308 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st61;
		case 62: goto tr95;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st61;
	goto tr0;
tr409:
#line 77 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st62;
st62:
	if ( ++p == pe )
		goto _out62;
case 62:
#line 1324 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr96;
		case 47: goto tr98;
		case 62: goto tr99;
		case 63: goto st62;
		case 95: goto st62;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr96;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st62;
		} else if ( (*p) >= 65 )
			goto st62;
	} else
		goto st62;
	goto tr0;
tr96:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _out63;
case 63:
#line 1352 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st63;
		case 47: goto st66;
		case 62: goto tr103;
		case 63: goto tr101;
		case 95: goto tr101;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st63;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr101;
		} else if ( (*p) >= 65 )
			goto tr101;
	} else
		goto tr101;
	goto tr0;
tr101:
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st64;
tr110:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st64;
st64:
	if ( ++p == pe )
		goto _out64;
case 64:
#line 1402 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr104;
		case 47: goto tr106;
		case 61: goto tr107;
		case 62: goto tr108;
		case 63: goto st64;
		case 95: goto st64;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr104;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st64;
		} else if ( (*p) >= 65 )
			goto st64;
	} else
		goto st64;
	goto tr39;
tr104:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st65;
tr136:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st65;
tr130:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st65;
st65:
	if ( ++p == pe )
		goto _out65;
case 65:
#line 1447 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st65;
		case 47: goto tr111;
		case 61: goto st67;
		case 62: goto tr113;
		case 63: goto tr110;
		case 95: goto tr110;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st65;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr110;
		} else if ( (*p) >= 65 )
			goto tr110;
	} else
		goto tr110;
	goto tr39;
tr98:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st66;
tr106:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st66;
tr111:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st66;
st66:
	if ( ++p == pe )
		goto _out66;
case 66:
#line 1490 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 62 )
		goto tr114;
	goto tr39;
tr107:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st67;
st67:
	if ( ++p == pe )
		goto _out67;
case 67:
#line 1502 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr116;
		case 32: goto tr116;
		case 34: goto st142;
		case 39: goto st143;
		case 47: goto tr120;
		case 60: goto tr39;
		case 62: goto tr113;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr117;
	} else if ( (*p) >= 9 )
		goto tr116;
	goto tr115;
tr115:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st68;
st68:
	if ( ++p == pe )
		goto _out68;
case 68:
#line 1526 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr122;
		case 32: goto tr122;
		case 47: goto tr124;
		case 60: goto tr39;
		case 62: goto tr125;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr123;
	} else if ( (*p) >= 9 )
		goto tr122;
	goto st68;
tr122:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st69;
tr164:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _out69;
case 69:
#line 1555 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st69;
		case 47: goto tr111;
		case 62: goto tr113;
		case 63: goto tr110;
		case 95: goto tr110;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st69;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr110;
		} else if ( (*p) >= 65 )
			goto tr110;
	} else
		goto tr110;
	goto tr39;
tr123:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st70;
tr151:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st70;
st70:
	if ( ++p == pe )
		goto _out70;
case 70:
#line 1590 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr122;
		case 32: goto tr122;
		case 47: goto tr128;
		case 60: goto tr39;
		case 62: goto tr129;
		case 63: goto tr127;
		case 95: goto tr127;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr123;
		} else if ( (*p) >= 9 )
			goto tr122;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr127;
		} else if ( (*p) >= 65 )
			goto tr127;
	} else
		goto tr127;
	goto st68;
tr127:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st71;
tr146:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st71;
st71:
	if ( ++p == pe )
		goto _out71;
case 71:
#line 1651 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr130;
		case 32: goto tr130;
		case 47: goto tr133;
		case 60: goto tr39;
		case 61: goto tr134;
		case 62: goto tr135;
		case 63: goto st71;
		case 95: goto st71;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr131;
		} else if ( (*p) >= 9 )
			goto tr130;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st71;
		} else if ( (*p) >= 65 )
			goto st71;
	} else
		goto st71;
	goto st68;
tr137:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st72;
tr131:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st72;
st72:
	if ( ++p == pe )
		goto _out72;
case 72:
#line 1697 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr136;
		case 32: goto tr136;
		case 47: goto tr128;
		case 60: goto tr39;
		case 61: goto st74;
		case 62: goto tr129;
		case 63: goto tr127;
		case 95: goto tr127;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr137;
		} else if ( (*p) >= 9 )
			goto tr136;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr127;
		} else if ( (*p) >= 65 )
			goto tr127;
	} else
		goto tr127;
	goto st68;
tr120:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr124:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr128:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st73;
tr133:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr143:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr147:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st73;
st73:
	if ( ++p == pe )
		goto _out73;
case 73:
#line 1796 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr122;
		case 32: goto tr122;
		case 47: goto tr124;
		case 60: goto tr39;
		case 62: goto tr125;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr123;
	} else if ( (*p) >= 9 )
		goto tr122;
	goto st68;
tr117:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st74;
tr134:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st74;
st74:
	if ( ++p == pe )
		goto _out74;
case 74:
#line 1822 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr139;
		case 32: goto tr139;
		case 34: goto st77;
		case 39: goto st141;
		case 47: goto tr143;
		case 60: goto tr39;
		case 62: goto tr125;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr140;
	} else if ( (*p) >= 9 )
		goto tr139;
	goto tr115;
tr144:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st75;
tr139:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st75;
st75:
	if ( ++p == pe )
		goto _out75;
case 75:
#line 1855 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr144;
		case 32: goto tr144;
		case 34: goto st142;
		case 39: goto st143;
		case 47: goto tr120;
		case 60: goto tr39;
		case 62: goto tr113;
		case 63: goto tr146;
		case 95: goto tr146;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr145;
		} else if ( (*p) >= 9 )
			goto tr144;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr146;
		} else if ( (*p) >= 65 )
			goto tr146;
	} else
		goto tr146;
	goto tr115;
tr145:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st76;
tr140:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st76;
st76:
	if ( ++p == pe )
		goto _out76;
case 76:
#line 1899 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr139;
		case 32: goto tr139;
		case 34: goto st77;
		case 39: goto st141;
		case 47: goto tr147;
		case 60: goto tr39;
		case 62: goto tr129;
		case 63: goto tr146;
		case 95: goto tr146;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr140;
		} else if ( (*p) >= 9 )
			goto tr139;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr146;
		} else if ( (*p) >= 65 )
			goto tr146;
	} else
		goto tr146;
	goto tr115;
st77:
	if ( ++p == pe )
		goto _out77;
case 77:
	switch( (*p) ) {
		case 13: goto tr149;
		case 32: goto tr149;
		case 34: goto tr151;
		case 47: goto tr152;
		case 60: goto tr153;
		case 62: goto tr154;
		case 92: goto tr155;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr150;
	} else if ( (*p) >= 9 )
		goto tr149;
	goto tr148;
tr148:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st78;
st78:
	if ( ++p == pe )
		goto _out78;
case 78:
#line 1953 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr157;
		case 32: goto tr157;
		case 34: goto tr151;
		case 47: goto tr159;
		case 60: goto st80;
		case 62: goto tr161;
		case 92: goto st94;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr158;
	} else if ( (*p) >= 9 )
		goto tr157;
	goto st78;
tr328:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st79;
tr157:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st79;
tr149:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st79;
tr169:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st79;
tr310:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st79;
st79:
	if ( ++p == pe )
		goto _out79;
case 79:
#line 2003 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st79;
		case 34: goto tr164;
		case 47: goto tr166;
		case 62: goto tr167;
		case 63: goto tr165;
		case 92: goto st81;
		case 95: goto tr165;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st79;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr165;
		} else if ( (*p) >= 65 )
			goto tr165;
	} else
		goto tr165;
	goto st80;
tr153:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st80;
st80:
	if ( ++p == pe )
		goto _out80;
case 80:
#line 2033 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr164;
		case 92: goto st81;
	}
	goto st80;
tr332:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st81;
st81:
	if ( ++p == pe )
		goto _out81;
case 81:
#line 2047 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr169;
		case 92: goto st81;
	}
	goto st80;
tr165:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st82;
tr329:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st82;
st82:
	if ( ++p == pe )
		goto _out82;
case 82:
#line 2089 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr170;
		case 34: goto tr164;
		case 47: goto tr172;
		case 61: goto tr173;
		case 62: goto tr174;
		case 63: goto st82;
		case 92: goto st81;
		case 95: goto st82;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr170;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st82;
		} else if ( (*p) >= 65 )
			goto st82;
	} else
		goto st82;
	goto st80;
tr170:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st83;
tr201:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st83;
tr195:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st83;
st83:
	if ( ++p == pe )
		goto _out83;
case 83:
#line 2136 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st83;
		case 34: goto tr164;
		case 47: goto tr166;
		case 61: goto st85;
		case 62: goto tr167;
		case 63: goto tr165;
		case 92: goto st81;
		case 95: goto tr165;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st83;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr165;
		} else if ( (*p) >= 65 )
			goto tr165;
	} else
		goto tr165;
	goto st80;
tr172:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
tr166:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
tr330:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
st84:
	if ( ++p == pe )
		goto _out84;
case 84:
#line 2185 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr164;
		case 62: goto tr177;
		case 92: goto st81;
	}
	goto st80;
tr154:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr161:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr167:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr174:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr177:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 70 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 12;}
	goto st209;
tr191:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr192:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr200:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
tr331:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st209;
st209:
	if ( ++p == pe )
		goto _out209;
case 209:
#line 2317 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr164;
		case 92: goto st81;
	}
	goto st80;
tr173:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st85;
st85:
	if ( ++p == pe )
		goto _out85;
case 85:
#line 2331 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr178;
		case 32: goto tr178;
		case 34: goto tr180;
		case 39: goto st140;
		case 47: goto tr182;
		case 60: goto st80;
		case 62: goto tr167;
		case 92: goto tr155;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr179;
	} else if ( (*p) >= 9 )
		goto tr178;
	goto tr148;
tr178:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st86;
st86:
	if ( ++p == pe )
		goto _out86;
case 86:
#line 2356 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr183;
		case 32: goto tr183;
		case 34: goto tr180;
		case 39: goto st140;
		case 47: goto tr182;
		case 60: goto st80;
		case 62: goto tr167;
		case 92: goto tr155;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr184;
	} else if ( (*p) >= 9 )
		goto tr183;
	goto tr148;
tr183:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st87;
tr186:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st87;
st87:
	if ( ++p == pe )
		goto _out87;
case 87:
#line 2390 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr183;
		case 32: goto tr183;
		case 34: goto tr180;
		case 39: goto st140;
		case 47: goto tr182;
		case 60: goto st80;
		case 62: goto tr167;
		case 63: goto tr185;
		case 92: goto tr155;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr184;
		} else if ( (*p) >= 9 )
			goto tr183;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr185;
	goto tr148;
tr184:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st88;
tr187:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st88;
st88:
	if ( ++p == pe )
		goto _out88;
case 88:
#line 2435 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr186;
		case 32: goto tr186;
		case 34: goto tr188;
		case 39: goto st96;
		case 47: goto tr190;
		case 60: goto st80;
		case 62: goto tr191;
		case 63: goto tr185;
		case 92: goto tr155;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr187;
		} else if ( (*p) >= 9 )
			goto tr186;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr185;
	goto tr148;
tr188:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st89;
st89:
	if ( ++p == pe )
		goto _out89;
case 89:
#line 2471 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr149;
		case 32: goto tr149;
		case 34: goto tr151;
		case 47: goto tr190;
		case 60: goto tr153;
		case 62: goto tr192;
		case 63: goto tr185;
		case 92: goto tr155;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr150;
		} else if ( (*p) >= 9 )
			goto tr149;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr185;
	goto tr148;
tr158:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st90;
tr150:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st90;
tr204:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st90;
tr275:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _out90;
case 90:
#line 2528 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr157;
		case 32: goto tr157;
		case 34: goto tr151;
		case 47: goto tr194;
		case 60: goto st80;
		case 62: goto tr191;
		case 63: goto tr193;
		case 92: goto st94;
		case 95: goto tr193;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr158;
		} else if ( (*p) >= 9 )
			goto tr157;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr193;
		} else if ( (*p) >= 65 )
			goto tr193;
	} else
		goto tr193;
	goto st78;
tr193:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st91;
tr185:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st91;
st91:
	if ( ++p == pe )
		goto _out91;
case 91:
#line 2591 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr195;
		case 32: goto tr195;
		case 34: goto tr151;
		case 47: goto tr198;
		case 60: goto st80;
		case 61: goto tr199;
		case 62: goto tr200;
		case 63: goto st91;
		case 92: goto st94;
		case 95: goto st91;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr196;
		} else if ( (*p) >= 9 )
			goto tr195;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st91;
		} else if ( (*p) >= 65 )
			goto st91;
	} else
		goto st91;
	goto st78;
tr202:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st92;
tr196:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st92;
st92:
	if ( ++p == pe )
		goto _out92;
case 92:
#line 2639 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr201;
		case 32: goto tr201;
		case 34: goto tr151;
		case 47: goto tr194;
		case 60: goto st80;
		case 61: goto st95;
		case 62: goto tr191;
		case 63: goto tr193;
		case 92: goto st94;
		case 95: goto tr193;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr202;
		} else if ( (*p) >= 9 )
			goto tr201;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr193;
		} else if ( (*p) >= 65 )
			goto tr193;
	} else
		goto tr193;
	goto st78;
tr182:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st93;
tr159:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st93;
tr194:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st93;
tr198:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st93;
tr152:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st93;
tr190:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st93;
st93:
	if ( ++p == pe )
		goto _out93;
case 93:
#line 2740 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr157;
		case 32: goto tr157;
		case 34: goto tr151;
		case 47: goto tr159;
		case 60: goto st80;
		case 62: goto tr161;
		case 92: goto st94;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr158;
	} else if ( (*p) >= 9 )
		goto tr157;
	goto st78;
tr155:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st94;
st94:
	if ( ++p == pe )
		goto _out94;
case 94:
#line 2764 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr157;
		case 32: goto tr157;
		case 34: goto tr204;
		case 47: goto tr159;
		case 60: goto st80;
		case 62: goto tr161;
		case 92: goto st94;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr158;
	} else if ( (*p) >= 9 )
		goto tr157;
	goto st78;
tr179:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st95;
tr199:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st95;
st95:
	if ( ++p == pe )
		goto _out95;
case 95:
#line 2792 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr186;
		case 32: goto tr186;
		case 34: goto tr188;
		case 39: goto st96;
		case 47: goto tr152;
		case 60: goto st80;
		case 62: goto tr161;
		case 92: goto tr155;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr187;
	} else if ( (*p) >= 9 )
		goto tr186;
	goto tr148;
st96:
	if ( ++p == pe )
		goto _out96;
case 96:
	switch( (*p) ) {
		case 13: goto tr206;
		case 32: goto tr206;
		case 34: goto tr208;
		case 39: goto tr204;
		case 47: goto tr209;
		case 60: goto tr210;
		case 62: goto tr211;
		case 92: goto tr212;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr207;
	} else if ( (*p) >= 9 )
		goto tr206;
	goto tr205;
tr205:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st97;
st97:
	if ( ++p == pe )
		goto _out97;
case 97:
#line 2837 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr214;
		case 32: goto tr214;
		case 34: goto tr216;
		case 39: goto tr204;
		case 47: goto tr217;
		case 60: goto st99;
		case 62: goto tr219;
		case 92: goto st129;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr215;
	} else if ( (*p) >= 9 )
		goto tr214;
	goto st97;
tr309:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st98;
tr214:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st98;
tr206:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st98;
tr293:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st98;
st98:
	if ( ++p == pe )
		goto _out98;
case 98:
#line 2882 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st98;
		case 34: goto tr222;
		case 39: goto tr169;
		case 47: goto tr224;
		case 62: goto tr225;
		case 63: goto tr223;
		case 92: goto st122;
		case 95: goto tr223;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st98;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr223;
		} else if ( (*p) >= 65 )
			goto tr223;
	} else
		goto tr223;
	goto st99;
tr210:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st99;
st99:
	if ( ++p == pe )
		goto _out99;
case 99:
#line 2913 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr222;
		case 39: goto tr169;
		case 92: goto st122;
	}
	goto st99;
tr323:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st100;
tr249:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st100;
tr319:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st100;
tr222:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st100;
tr315:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st100;
st100:
	if ( ++p == pe )
		goto _out100;
case 100:
#line 2954 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st100;
		case 39: goto tr164;
		case 47: goto tr230;
		case 62: goto tr231;
		case 63: goto tr229;
		case 92: goto st102;
		case 95: goto tr229;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st100;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr229;
		} else if ( (*p) >= 65 )
			goto tr229;
	} else
		goto tr229;
	goto st101;
tr321:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st101;
st101:
	if ( ++p == pe )
		goto _out101;
case 101:
#line 2984 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr164;
		case 92: goto st102;
	}
	goto st101;
tr327:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st102;
st102:
	if ( ++p == pe )
		goto _out102;
case 102:
#line 2998 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr222;
		case 92: goto st102;
	}
	goto st101;
tr229:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st103;
tr324:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st103;
st103:
	if ( ++p == pe )
		goto _out103;
case 103:
#line 3040 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr233;
		case 39: goto tr164;
		case 47: goto tr235;
		case 61: goto tr236;
		case 62: goto tr237;
		case 63: goto st103;
		case 92: goto st102;
		case 95: goto st103;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr233;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st103;
		} else if ( (*p) >= 65 )
			goto st103;
	} else
		goto st103;
	goto st101;
tr233:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st104;
tr263:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st104;
tr257:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st104;
st104:
	if ( ++p == pe )
		goto _out104;
case 104:
#line 3087 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st104;
		case 39: goto tr164;
		case 47: goto tr230;
		case 61: goto st106;
		case 62: goto tr231;
		case 63: goto tr229;
		case 92: goto st102;
		case 95: goto tr229;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st104;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr229;
		} else if ( (*p) >= 65 )
			goto tr229;
	} else
		goto tr229;
	goto st101;
tr235:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st105;
tr230:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st105;
tr325:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st105;
st105:
	if ( ++p == pe )
		goto _out105;
case 105:
#line 3136 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr164;
		case 62: goto tr240;
		case 92: goto st102;
	}
	goto st101;
tr333:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr252:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr231:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr237:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr240:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 70 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 12;}
	goto st210;
tr256:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr322:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr262:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
tr326:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st210;
st210:
	if ( ++p == pe )
		goto _out210;
case 210:
#line 3268 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr164;
		case 92: goto st102;
	}
	goto st101;
tr236:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st106;
st106:
	if ( ++p == pe )
		goto _out106;
case 106:
#line 3282 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr242;
		case 32: goto tr242;
		case 34: goto st136;
		case 39: goto tr245;
		case 47: goto tr246;
		case 60: goto st101;
		case 62: goto tr231;
		case 92: goto tr247;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr243;
	} else if ( (*p) >= 9 )
		goto tr242;
	goto tr241;
tr241:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st107;
st107:
	if ( ++p == pe )
		goto _out107;
case 107:
#line 3307 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr249;
		case 32: goto tr249;
		case 39: goto tr151;
		case 47: goto tr251;
		case 60: goto st101;
		case 62: goto tr252;
		case 92: goto st112;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr250;
	} else if ( (*p) >= 9 )
		goto tr249;
	goto st107;
tr250:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st108;
tr320:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st108;
tr216:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st108;
tr208:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st108;
st108:
	if ( ++p == pe )
		goto _out108;
case 108:
#line 3353 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr249;
		case 32: goto tr249;
		case 39: goto tr151;
		case 47: goto tr255;
		case 60: goto st101;
		case 62: goto tr256;
		case 63: goto tr254;
		case 92: goto st112;
		case 95: goto tr254;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr250;
		} else if ( (*p) >= 9 )
			goto tr249;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr254;
		} else if ( (*p) >= 65 )
			goto tr254;
	} else
		goto tr254;
	goto st107;
tr254:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st109;
tr273:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st109;
st109:
	if ( ++p == pe )
		goto _out109;
case 109:
#line 3416 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr257;
		case 32: goto tr257;
		case 39: goto tr151;
		case 47: goto tr260;
		case 60: goto st101;
		case 61: goto tr261;
		case 62: goto tr262;
		case 63: goto st109;
		case 92: goto st112;
		case 95: goto st109;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr258;
		} else if ( (*p) >= 9 )
			goto tr257;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st109;
		} else if ( (*p) >= 65 )
			goto st109;
	} else
		goto st109;
	goto st107;
tr264:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st110;
tr258:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st110;
st110:
	if ( ++p == pe )
		goto _out110;
case 110:
#line 3464 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr263;
		case 32: goto tr263;
		case 39: goto tr151;
		case 47: goto tr255;
		case 60: goto st101;
		case 61: goto st113;
		case 62: goto tr256;
		case 63: goto tr254;
		case 92: goto st112;
		case 95: goto tr254;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr264;
		} else if ( (*p) >= 9 )
			goto tr263;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr254;
		} else if ( (*p) >= 65 )
			goto tr254;
	} else
		goto tr254;
	goto st107;
tr246:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr251:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr255:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st111;
tr260:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr270:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr274:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st111;
st111:
	if ( ++p == pe )
		goto _out111;
case 111:
#line 3565 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr249;
		case 32: goto tr249;
		case 39: goto tr151;
		case 47: goto tr251;
		case 60: goto st101;
		case 62: goto tr252;
		case 92: goto st112;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr250;
	} else if ( (*p) >= 9 )
		goto tr249;
	goto st107;
tr247:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st112;
st112:
	if ( ++p == pe )
		goto _out112;
case 112:
#line 3589 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr249;
		case 32: goto tr249;
		case 39: goto tr216;
		case 47: goto tr251;
		case 60: goto st101;
		case 62: goto tr252;
		case 92: goto st112;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr250;
	} else if ( (*p) >= 9 )
		goto tr249;
	goto st107;
tr243:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st113;
tr261:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st113;
st113:
	if ( ++p == pe )
		goto _out113;
case 113:
#line 3617 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr266;
		case 32: goto tr266;
		case 34: goto st116;
		case 39: goto tr269;
		case 47: goto tr270;
		case 60: goto st101;
		case 62: goto tr252;
		case 92: goto tr247;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr267;
	} else if ( (*p) >= 9 )
		goto tr266;
	goto tr241;
tr271:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st114;
tr266:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st114;
st114:
	if ( ++p == pe )
		goto _out114;
case 114:
#line 3651 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr271;
		case 32: goto tr271;
		case 34: goto st136;
		case 39: goto tr245;
		case 47: goto tr246;
		case 60: goto st101;
		case 62: goto tr231;
		case 63: goto tr273;
		case 92: goto tr247;
		case 95: goto tr273;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr272;
		} else if ( (*p) >= 9 )
			goto tr271;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr273;
		} else if ( (*p) >= 65 )
			goto tr273;
	} else
		goto tr273;
	goto tr241;
tr272:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st115;
tr267:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st115;
st115:
	if ( ++p == pe )
		goto _out115;
case 115:
#line 3696 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr266;
		case 32: goto tr266;
		case 34: goto st116;
		case 39: goto tr269;
		case 47: goto tr274;
		case 60: goto st101;
		case 62: goto tr256;
		case 63: goto tr273;
		case 92: goto tr247;
		case 95: goto tr273;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr267;
		} else if ( (*p) >= 9 )
			goto tr266;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr273;
		} else if ( (*p) >= 65 )
			goto tr273;
	} else
		goto tr273;
	goto tr241;
st116:
	if ( ++p == pe )
		goto _out116;
case 116:
	switch( (*p) ) {
		case 13: goto tr206;
		case 32: goto tr206;
		case 34: goto tr216;
		case 39: goto tr275;
		case 47: goto tr209;
		case 60: goto tr210;
		case 62: goto tr211;
		case 92: goto tr212;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr207;
	} else if ( (*p) >= 9 )
		goto tr206;
	goto tr205;
tr215:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st117;
tr207:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st117;
tr308:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st117;
st117:
	if ( ++p == pe )
		goto _out117;
case 117:
#line 3768 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr214;
		case 32: goto tr214;
		case 34: goto tr216;
		case 39: goto tr204;
		case 47: goto tr277;
		case 60: goto st99;
		case 62: goto tr278;
		case 63: goto tr276;
		case 92: goto st129;
		case 95: goto tr276;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr215;
		} else if ( (*p) >= 9 )
			goto tr214;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr276;
		} else if ( (*p) >= 65 )
			goto tr276;
	} else
		goto tr276;
	goto st97;
tr276:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st118;
tr301:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st118;
st118:
	if ( ++p == pe )
		goto _out118;
case 118:
#line 3832 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr279;
		case 32: goto tr279;
		case 34: goto tr216;
		case 39: goto tr204;
		case 47: goto tr282;
		case 60: goto st99;
		case 61: goto tr283;
		case 62: goto tr284;
		case 63: goto st118;
		case 92: goto st129;
		case 95: goto st118;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr280;
		} else if ( (*p) >= 9 )
			goto tr279;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st118;
		} else if ( (*p) >= 65 )
			goto st118;
	} else
		goto st118;
	goto st97;
tr287:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st119;
tr316:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st119;
tr279:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st119;
st119:
	if ( ++p == pe )
		goto _out119;
case 119:
#line 3885 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st119;
		case 34: goto tr222;
		case 39: goto tr169;
		case 47: goto tr224;
		case 61: goto st123;
		case 62: goto tr225;
		case 63: goto tr223;
		case 92: goto st122;
		case 95: goto tr223;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st119;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr223;
		} else if ( (*p) >= 65 )
			goto tr223;
	} else
		goto tr223;
	goto st99;
tr223:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st120;
tr311:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 94 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 79 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st120;
st120:
	if ( ++p == pe )
		goto _out120;
case 120:
#line 3945 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr287;
		case 34: goto tr222;
		case 39: goto tr169;
		case 47: goto tr289;
		case 61: goto tr290;
		case 62: goto tr291;
		case 63: goto st120;
		case 92: goto st122;
		case 95: goto st120;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr287;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st120;
		} else if ( (*p) >= 65 )
			goto st120;
	} else
		goto st120;
	goto st99;
tr289:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st121;
tr224:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st121;
tr312:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st121;
st121:
	if ( ++p == pe )
		goto _out121;
case 121:
#line 3995 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr222;
		case 39: goto tr169;
		case 62: goto tr292;
		case 92: goto st122;
	}
	goto st99;
tr211:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr219:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr225:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr291:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr292:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 70 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 12;}
	goto st211;
tr278:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr307:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr284:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
tr313:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 68 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 10;}
	goto st211;
st211:
	if ( ++p == pe )
		goto _out211;
case 211:
#line 4128 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr222;
		case 39: goto tr169;
		case 92: goto st122;
	}
	goto st99;
tr314:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st122;
st122:
	if ( ++p == pe )
		goto _out122;
case 122:
#line 4143 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr293;
		case 39: goto tr293;
		case 92: goto st122;
	}
	goto st99;
tr290:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st123;
st123:
	if ( ++p == pe )
		goto _out123;
case 123:
#line 4158 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr294;
		case 32: goto tr294;
		case 34: goto tr296;
		case 39: goto tr297;
		case 47: goto tr298;
		case 60: goto st99;
		case 62: goto tr225;
		case 92: goto tr212;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr295;
	} else if ( (*p) >= 9 )
		goto tr294;
	goto tr205;
tr294:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st124;
st124:
	if ( ++p == pe )
		goto _out124;
case 124:
#line 4183 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr299;
		case 32: goto tr299;
		case 34: goto tr296;
		case 39: goto tr297;
		case 47: goto tr298;
		case 60: goto st99;
		case 62: goto tr225;
		case 92: goto tr212;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr300;
	} else if ( (*p) >= 9 )
		goto tr299;
	goto tr205;
tr299:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st125;
tr302:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st125;
st125:
	if ( ++p == pe )
		goto _out125;
case 125:
#line 4217 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr299;
		case 32: goto tr299;
		case 34: goto tr296;
		case 39: goto tr297;
		case 47: goto tr298;
		case 60: goto st99;
		case 62: goto tr225;
		case 63: goto tr301;
		case 92: goto tr212;
		case 95: goto tr301;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr300;
		} else if ( (*p) >= 9 )
			goto tr299;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr301;
		} else if ( (*p) >= 65 )
			goto tr301;
	} else
		goto tr301;
	goto tr205;
tr300:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st126;
tr303:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st126;
st126:
	if ( ++p == pe )
		goto _out126;
case 126:
#line 4262 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr302;
		case 32: goto tr302;
		case 34: goto tr304;
		case 39: goto tr305;
		case 47: goto tr306;
		case 60: goto st99;
		case 62: goto tr278;
		case 63: goto tr301;
		case 92: goto tr212;
		case 95: goto tr301;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr303;
		} else if ( (*p) >= 9 )
			goto tr302;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr301;
		} else if ( (*p) >= 65 )
			goto tr301;
	} else
		goto tr301;
	goto tr205;
tr304:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st127;
st127:
	if ( ++p == pe )
		goto _out127;
case 127:
#line 4298 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr206;
		case 32: goto tr206;
		case 34: goto tr216;
		case 39: goto tr275;
		case 47: goto tr306;
		case 60: goto tr210;
		case 62: goto tr307;
		case 63: goto tr301;
		case 92: goto tr212;
		case 95: goto tr301;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr207;
		} else if ( (*p) >= 9 )
			goto tr206;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr301;
		} else if ( (*p) >= 65 )
			goto tr301;
	} else
		goto tr301;
	goto tr205;
tr298:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st128;
tr217:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st128;
tr277:
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st128;
tr282:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st128;
tr209:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st128;
tr306:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 101 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st128;
st128:
	if ( ++p == pe )
		goto _out128;
case 128:
#line 4399 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr214;
		case 32: goto tr214;
		case 34: goto tr216;
		case 39: goto tr204;
		case 47: goto tr217;
		case 60: goto st99;
		case 62: goto tr219;
		case 92: goto st129;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr215;
	} else if ( (*p) >= 9 )
		goto tr214;
	goto st97;
tr212:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st129;
st129:
	if ( ++p == pe )
		goto _out129;
case 129:
#line 4424 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr214;
		case 32: goto tr214;
		case 34: goto tr308;
		case 39: goto tr308;
		case 47: goto tr217;
		case 60: goto st99;
		case 62: goto tr219;
		case 92: goto st129;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr215;
	} else if ( (*p) >= 9 )
		goto tr214;
	goto st97;
tr305:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st130;
st130:
	if ( ++p == pe )
		goto _out130;
case 130:
#line 4449 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr206;
		case 32: goto tr206;
		case 34: goto tr208;
		case 39: goto tr204;
		case 47: goto tr306;
		case 60: goto tr210;
		case 62: goto tr307;
		case 63: goto tr301;
		case 92: goto tr212;
		case 95: goto tr301;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr207;
		} else if ( (*p) >= 9 )
			goto tr206;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr301;
		} else if ( (*p) >= 65 )
			goto tr301;
	} else
		goto tr301;
	goto tr205;
tr296:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st131;
st131:
	if ( ++p == pe )
		goto _out131;
case 131:
#line 4485 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr309;
		case 34: goto tr222;
		case 39: goto tr310;
		case 47: goto tr312;
		case 62: goto tr313;
		case 63: goto tr311;
		case 92: goto tr314;
		case 95: goto tr311;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr309;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr311;
		} else if ( (*p) >= 65 )
			goto tr311;
	} else
		goto tr311;
	goto tr210;
tr297:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st132;
st132:
	if ( ++p == pe )
		goto _out132;
case 132:
#line 4516 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr309;
		case 34: goto tr315;
		case 39: goto tr169;
		case 47: goto tr312;
		case 62: goto tr313;
		case 63: goto tr311;
		case 92: goto tr314;
		case 95: goto tr311;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr309;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr311;
		} else if ( (*p) >= 65 )
			goto tr311;
	} else
		goto tr311;
	goto tr210;
tr295:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st133;
tr283:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st133;
st133:
	if ( ++p == pe )
		goto _out133;
case 133:
#line 4551 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr302;
		case 32: goto tr302;
		case 34: goto tr304;
		case 39: goto tr305;
		case 47: goto tr209;
		case 60: goto st99;
		case 62: goto tr219;
		case 92: goto tr212;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr303;
	} else if ( (*p) >= 9 )
		goto tr302;
	goto tr205;
tr317:
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st134;
tr280:
#line 87 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 83 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    if (*(p-1) == '"' || *(p-1) == '\'') { SET(aval, p-1); }
    else { SET(aval, p); }
  }
	goto st134;
st134:
	if ( ++p == pe )
		goto _out134;
case 134:
#line 4588 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr316;
		case 32: goto tr316;
		case 34: goto tr216;
		case 39: goto tr204;
		case 47: goto tr277;
		case 60: goto st99;
		case 61: goto st133;
		case 62: goto tr278;
		case 63: goto tr276;
		case 92: goto st129;
		case 95: goto tr276;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr317;
		} else if ( (*p) >= 9 )
			goto tr316;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr276;
		} else if ( (*p) >= 65 )
			goto tr276;
	} else
		goto tr276;
	goto st97;
tr269:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st135;
st135:
	if ( ++p == pe )
		goto _out135;
case 135:
#line 4625 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr319;
		case 32: goto tr319;
		case 39: goto tr151;
		case 47: goto tr274;
		case 60: goto tr321;
		case 62: goto tr322;
		case 63: goto tr273;
		case 92: goto tr247;
		case 95: goto tr273;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 12 )
				goto tr320;
		} else if ( (*p) >= 9 )
			goto tr319;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr273;
		} else if ( (*p) >= 65 )
			goto tr273;
	} else
		goto tr273;
	goto tr241;
st136:
	if ( ++p == pe )
		goto _out136;
case 136:
	switch( (*p) ) {
		case 34: goto tr222;
		case 39: goto tr310;
		case 92: goto tr314;
	}
	goto tr210;
tr245:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st137;
st137:
	if ( ++p == pe )
		goto _out137;
case 137:
#line 4670 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr323;
		case 39: goto tr164;
		case 47: goto tr325;
		case 62: goto tr326;
		case 63: goto tr324;
		case 92: goto tr327;
		case 95: goto tr324;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr323;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr324;
		} else if ( (*p) >= 65 )
			goto tr324;
	} else
		goto tr324;
	goto tr321;
tr242:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st138;
st138:
	if ( ++p == pe )
		goto _out138;
case 138:
#line 4700 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr271;
		case 32: goto tr271;
		case 34: goto st136;
		case 39: goto tr245;
		case 47: goto tr246;
		case 60: goto st101;
		case 62: goto tr231;
		case 92: goto tr247;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr272;
	} else if ( (*p) >= 9 )
		goto tr271;
	goto tr241;
tr180:
#line 82 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st139;
st139:
	if ( ++p == pe )
		goto _out139;
case 139:
#line 4725 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr328;
		case 34: goto tr164;
		case 47: goto tr330;
		case 62: goto tr331;
		case 63: goto tr329;
		case 92: goto tr332;
		case 95: goto tr329;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr328;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr329;
		} else if ( (*p) >= 65 )
			goto tr329;
	} else
		goto tr329;
	goto tr153;
st140:
	if ( ++p == pe )
		goto _out140;
case 140:
	switch( (*p) ) {
		case 34: goto tr315;
		case 39: goto tr169;
		case 92: goto tr314;
	}
	goto tr210;
st141:
	if ( ++p == pe )
		goto _out141;
case 141:
	switch( (*p) ) {
		case 13: goto tr319;
		case 32: goto tr319;
		case 39: goto tr151;
		case 47: goto tr270;
		case 60: goto tr321;
		case 62: goto tr333;
		case 92: goto tr247;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr320;
	} else if ( (*p) >= 9 )
		goto tr319;
	goto tr241;
st142:
	if ( ++p == pe )
		goto _out142;
case 142:
	switch( (*p) ) {
		case 34: goto tr164;
		case 92: goto tr332;
	}
	goto tr153;
st143:
	if ( ++p == pe )
		goto _out143;
case 143:
	switch( (*p) ) {
		case 39: goto tr164;
		case 92: goto tr327;
	}
	goto tr321;
tr116:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st144;
st144:
	if ( ++p == pe )
		goto _out144;
case 144:
#line 4802 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 13: goto tr144;
		case 32: goto tr144;
		case 34: goto st142;
		case 39: goto st143;
		case 47: goto tr120;
		case 60: goto tr39;
		case 62: goto tr113;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 12 )
			goto tr145;
	} else if ( (*p) >= 9 )
		goto tr144;
	goto tr115;
st145:
	if ( ++p == pe )
		goto _out145;
case 145:
	switch( (*p) ) {
		case 58: goto tr334;
		case 95: goto tr334;
		case 120: goto tr335;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr334;
	} else if ( (*p) >= 65 )
		goto tr334;
	goto tr0;
tr334:
#line 46 "ext/hpricot_scan/hpricot_scan.rl"
	{ TEXT_PASS(); }
	goto st146;
st146:
	if ( ++p == pe )
		goto _out146;
case 146:
#line 4841 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st212;
		case 63: goto st146;
		case 95: goto st146;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st146;
		} else if ( (*p) >= 9 )
			goto st212;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st146;
		} else if ( (*p) >= 65 )
			goto st146;
	} else
		goto st146;
	goto tr0;
st212:
	if ( ++p == pe )
		goto _out212;
case 212:
	if ( (*p) == 32 )
		goto st212;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st212;
	goto tr411;
tr335:
#line 46 "ext/hpricot_scan/hpricot_scan.rl"
	{ TEXT_PASS(); }
	goto st147;
st147:
	if ( ++p == pe )
		goto _out147;
case 147:
#line 4879 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st212;
		case 63: goto st146;
		case 95: goto st146;
		case 109: goto st148;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st146;
		} else if ( (*p) >= 9 )
			goto st212;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st146;
		} else if ( (*p) >= 65 )
			goto st146;
	} else
		goto st146;
	goto tr0;
st148:
	if ( ++p == pe )
		goto _out148;
case 148:
	switch( (*p) ) {
		case 32: goto st212;
		case 63: goto st146;
		case 95: goto st146;
		case 108: goto st149;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st146;
		} else if ( (*p) >= 9 )
			goto st212;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st146;
		} else if ( (*p) >= 65 )
			goto st146;
	} else
		goto st146;
	goto tr0;
st149:
	if ( ++p == pe )
		goto _out149;
case 149:
	switch( (*p) ) {
		case 32: goto tr340;
		case 63: goto st146;
		case 95: goto st146;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st146;
		} else if ( (*p) >= 9 )
			goto tr340;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st146;
		} else if ( (*p) >= 65 )
			goto st146;
	} else
		goto st146;
	goto tr0;
tr340:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
	goto st213;
st213:
	if ( ++p == pe )
		goto _out213;
case 213:
#line 4958 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr340;
		case 118: goto st150;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr340;
	goto tr411;
st150:
	if ( ++p == pe )
		goto _out150;
case 150:
	if ( (*p) == 101 )
		goto st151;
	goto tr341;
st151:
	if ( ++p == pe )
		goto _out151;
case 151:
	if ( (*p) == 114 )
		goto st152;
	goto tr341;
st152:
	if ( ++p == pe )
		goto _out152;
case 152:
	if ( (*p) == 115 )
		goto st153;
	goto tr341;
st153:
	if ( ++p == pe )
		goto _out153;
case 153:
	if ( (*p) == 105 )
		goto st154;
	goto tr341;
st154:
	if ( ++p == pe )
		goto _out154;
case 154:
	if ( (*p) == 111 )
		goto st155;
	goto tr341;
st155:
	if ( ++p == pe )
		goto _out155;
case 155:
	if ( (*p) == 110 )
		goto st156;
	goto tr341;
st156:
	if ( ++p == pe )
		goto _out156;
case 156:
	switch( (*p) ) {
		case 32: goto st156;
		case 61: goto st157;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st156;
	goto tr341;
st157:
	if ( ++p == pe )
		goto _out157;
case 157:
	switch( (*p) ) {
		case 32: goto st157;
		case 34: goto st158;
		case 39: goto st200;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st157;
	goto tr341;
st158:
	if ( ++p == pe )
		goto _out158;
case 158:
	if ( (*p) == 95 )
		goto tr351;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto tr351;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr351;
		} else if ( (*p) >= 65 )
			goto tr351;
	} else
		goto tr351;
	goto tr341;
tr351:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st159;
st159:
	if ( ++p == pe )
		goto _out159;
case 159:
#line 5057 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr352;
		case 95: goto st159;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st159;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st159;
		} else if ( (*p) >= 65 )
			goto st159;
	} else
		goto st159;
	goto tr341;
tr352:
#line 88 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("version"), aval); }
	goto st160;
st160:
	if ( ++p == pe )
		goto _out160;
case 160:
#line 5082 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st161;
		case 63: goto st162;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st161;
	goto tr341;
st161:
	if ( ++p == pe )
		goto _out161;
case 161:
	switch( (*p) ) {
		case 32: goto st161;
		case 63: goto st162;
		case 101: goto st163;
		case 115: goto st176;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st161;
	goto tr341;
st162:
	if ( ++p == pe )
		goto _out162;
case 162:
	if ( (*p) == 62 )
		goto tr358;
	goto tr341;
st163:
	if ( ++p == pe )
		goto _out163;
case 163:
	if ( (*p) == 110 )
		goto st164;
	goto tr341;
st164:
	if ( ++p == pe )
		goto _out164;
case 164:
	if ( (*p) == 99 )
		goto st165;
	goto tr341;
st165:
	if ( ++p == pe )
		goto _out165;
case 165:
	if ( (*p) == 111 )
		goto st166;
	goto tr341;
st166:
	if ( ++p == pe )
		goto _out166;
case 166:
	if ( (*p) == 100 )
		goto st167;
	goto tr341;
st167:
	if ( ++p == pe )
		goto _out167;
case 167:
	if ( (*p) == 105 )
		goto st168;
	goto tr341;
st168:
	if ( ++p == pe )
		goto _out168;
case 168:
	if ( (*p) == 110 )
		goto st169;
	goto tr341;
st169:
	if ( ++p == pe )
		goto _out169;
case 169:
	if ( (*p) == 103 )
		goto st170;
	goto tr341;
st170:
	if ( ++p == pe )
		goto _out170;
case 170:
	switch( (*p) ) {
		case 32: goto st170;
		case 61: goto st171;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st170;
	goto tr341;
st171:
	if ( ++p == pe )
		goto _out171;
case 171:
	switch( (*p) ) {
		case 32: goto st171;
		case 34: goto st172;
		case 39: goto st198;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st171;
	goto tr341;
st172:
	if ( ++p == pe )
		goto _out172;
case 172:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr369;
	} else if ( (*p) >= 65 )
		goto tr369;
	goto tr341;
tr369:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st173;
st173:
	if ( ++p == pe )
		goto _out173;
case 173:
#line 5200 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr370;
		case 95: goto st173;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st173;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st173;
		} else if ( (*p) >= 65 )
			goto st173;
	} else
		goto st173;
	goto tr341;
tr370:
#line 89 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("encoding"), aval); }
	goto st174;
st174:
	if ( ++p == pe )
		goto _out174;
case 174:
#line 5225 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st175;
		case 63: goto st162;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st175;
	goto tr341;
st175:
	if ( ++p == pe )
		goto _out175;
case 175:
	switch( (*p) ) {
		case 32: goto st175;
		case 63: goto st162;
		case 115: goto st176;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st175;
	goto tr341;
st176:
	if ( ++p == pe )
		goto _out176;
case 176:
	if ( (*p) == 116 )
		goto st177;
	goto tr341;
st177:
	if ( ++p == pe )
		goto _out177;
case 177:
	if ( (*p) == 97 )
		goto st178;
	goto tr341;
st178:
	if ( ++p == pe )
		goto _out178;
case 178:
	if ( (*p) == 110 )
		goto st179;
	goto tr341;
st179:
	if ( ++p == pe )
		goto _out179;
case 179:
	if ( (*p) == 100 )
		goto st180;
	goto tr341;
st180:
	if ( ++p == pe )
		goto _out180;
case 180:
	if ( (*p) == 97 )
		goto st181;
	goto tr341;
st181:
	if ( ++p == pe )
		goto _out181;
case 181:
	if ( (*p) == 108 )
		goto st182;
	goto tr341;
st182:
	if ( ++p == pe )
		goto _out182;
case 182:
	if ( (*p) == 111 )
		goto st183;
	goto tr341;
st183:
	if ( ++p == pe )
		goto _out183;
case 183:
	if ( (*p) == 110 )
		goto st184;
	goto tr341;
st184:
	if ( ++p == pe )
		goto _out184;
case 184:
	if ( (*p) == 101 )
		goto st185;
	goto tr341;
st185:
	if ( ++p == pe )
		goto _out185;
case 185:
	switch( (*p) ) {
		case 32: goto st185;
		case 61: goto st186;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st185;
	goto tr341;
st186:
	if ( ++p == pe )
		goto _out186;
case 186:
	switch( (*p) ) {
		case 32: goto st186;
		case 34: goto st187;
		case 39: goto st193;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st186;
	goto tr341;
st187:
	if ( ++p == pe )
		goto _out187;
case 187:
	switch( (*p) ) {
		case 110: goto tr385;
		case 121: goto tr386;
	}
	goto tr341;
tr385:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st188;
st188:
	if ( ++p == pe )
		goto _out188;
case 188:
#line 5348 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 111 )
		goto st189;
	goto tr341;
st189:
	if ( ++p == pe )
		goto _out189;
case 189:
	if ( (*p) == 34 )
		goto tr388;
	goto tr341;
tr388:
#line 90 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("standalone"), aval); }
	goto st190;
st190:
	if ( ++p == pe )
		goto _out190;
case 190:
#line 5367 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st190;
		case 63: goto st162;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st190;
	goto tr341;
tr386:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st191;
st191:
	if ( ++p == pe )
		goto _out191;
case 191:
#line 5383 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 101 )
		goto st192;
	goto tr341;
st192:
	if ( ++p == pe )
		goto _out192;
case 192:
	if ( (*p) == 115 )
		goto st189;
	goto tr341;
st193:
	if ( ++p == pe )
		goto _out193;
case 193:
	switch( (*p) ) {
		case 110: goto tr391;
		case 121: goto tr392;
	}
	goto tr341;
tr391:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st194;
st194:
	if ( ++p == pe )
		goto _out194;
case 194:
#line 5411 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 111 )
		goto st195;
	goto tr341;
st195:
	if ( ++p == pe )
		goto _out195;
case 195:
	if ( (*p) == 39 )
		goto tr388;
	goto tr341;
tr392:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st196;
st196:
	if ( ++p == pe )
		goto _out196;
case 196:
#line 5430 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 101 )
		goto st197;
	goto tr341;
st197:
	if ( ++p == pe )
		goto _out197;
case 197:
	if ( (*p) == 115 )
		goto st195;
	goto tr341;
st198:
	if ( ++p == pe )
		goto _out198;
case 198:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr395;
	} else if ( (*p) >= 65 )
		goto tr395;
	goto tr341;
tr395:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st199;
st199:
	if ( ++p == pe )
		goto _out199;
case 199:
#line 5459 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr370;
		case 95: goto st199;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st199;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st199;
		} else if ( (*p) >= 65 )
			goto st199;
	} else
		goto st199;
	goto tr341;
st200:
	if ( ++p == pe )
		goto _out200;
case 200:
	if ( (*p) == 95 )
		goto tr397;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto tr397;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr397;
		} else if ( (*p) >= 65 )
			goto tr397;
	} else
		goto tr397;
	goto tr341;
tr397:
#line 78 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st201;
st201:
	if ( ++p == pe )
		goto _out201;
case 201:
#line 5502 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr352;
		case 95: goto st201;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st201;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st201;
		} else if ( (*p) >= 65 )
			goto st201;
	} else
		goto st201;
	goto tr341;
tr399:
#line 51 "ext/hpricot_scan/hpricot_scan.rl"
	{{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st214;
tr400:
#line 50 "ext/hpricot_scan/hpricot_scan.rl"
	{ EBLK(comment, 3); {goto st204;} }
#line 50 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{p = ((tokend))-1;}}
	goto st214;
tr413:
#line 51 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st214;
tr414:
#line 9 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
#line 51 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st214;
tr416:
#line 51 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st214;
st214:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out214;
case 214:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = p;}
#line 5551 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr414;
		case 45: goto tr415;
	}
	goto tr413;
tr415:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
	goto st215;
st215:
	if ( ++p == pe )
		goto _out215;
case 215:
#line 5565 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 45 )
		goto st202;
	goto tr416;
st202:
	if ( ++p == pe )
		goto _out202;
case 202:
	if ( (*p) == 62 )
		goto tr400;
	goto tr399;
tr401:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st216;
tr402:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ EBLK(cdata, 3); {goto st204;} }
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{p = ((tokend))-1;}}
	goto st216;
tr418:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st216;
tr419:
#line 9 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st216;
tr421:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st216;
st216:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out216;
case 216:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = p;}
#line 5608 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr419;
		case 93: goto tr420;
	}
	goto tr418;
tr420:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
	goto st217;
st217:
	if ( ++p == pe )
		goto _out217;
case 217:
#line 5622 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 93 )
		goto st203;
	goto tr421;
st203:
	if ( ++p == pe )
		goto _out203;
case 203:
	if ( (*p) == 62 )
		goto tr402;
	goto tr401;
tr423:
#line 61 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st218;
tr424:
#line 9 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
#line 61 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st218;
tr426:
#line 61 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ TEXT_PASS(); }{p = ((tokend))-1;}}
	goto st218;
tr427:
#line 60 "ext/hpricot_scan/hpricot_scan.rl"
	{ EBLK(procins, 2); {goto st204;} }
#line 60 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{p = ((tokend))-1;}}
	goto st218;
st218:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out218;
case 218:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = p;}
#line 5661 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr424;
		case 63: goto st219;
	}
	goto tr423;
st219:
	if ( ++p == pe )
		goto _out219;
case 219:
	if ( (*p) == 62 )
		goto tr427;
	goto tr426;
	}
	_out204: cs = 204; goto _out; 
	_out205: cs = 205; goto _out; 
	_out0: cs = 0; goto _out; 
	_out1: cs = 1; goto _out; 
	_out2: cs = 2; goto _out; 
	_out3: cs = 3; goto _out; 
	_out4: cs = 4; goto _out; 
	_out5: cs = 5; goto _out; 
	_out6: cs = 6; goto _out; 
	_out7: cs = 7; goto _out; 
	_out8: cs = 8; goto _out; 
	_out9: cs = 9; goto _out; 
	_out10: cs = 10; goto _out; 
	_out11: cs = 11; goto _out; 
	_out12: cs = 12; goto _out; 
	_out13: cs = 13; goto _out; 
	_out14: cs = 14; goto _out; 
	_out15: cs = 15; goto _out; 
	_out16: cs = 16; goto _out; 
	_out17: cs = 17; goto _out; 
	_out18: cs = 18; goto _out; 
	_out19: cs = 19; goto _out; 
	_out20: cs = 20; goto _out; 
	_out21: cs = 21; goto _out; 
	_out22: cs = 22; goto _out; 
	_out23: cs = 23; goto _out; 
	_out24: cs = 24; goto _out; 
	_out25: cs = 25; goto _out; 
	_out26: cs = 26; goto _out; 
	_out27: cs = 27; goto _out; 
	_out28: cs = 28; goto _out; 
	_out29: cs = 29; goto _out; 
	_out30: cs = 30; goto _out; 
	_out31: cs = 31; goto _out; 
	_out32: cs = 32; goto _out; 
	_out33: cs = 33; goto _out; 
	_out34: cs = 34; goto _out; 
	_out35: cs = 35; goto _out; 
	_out36: cs = 36; goto _out; 
	_out37: cs = 37; goto _out; 
	_out38: cs = 38; goto _out; 
	_out39: cs = 39; goto _out; 
	_out206: cs = 206; goto _out; 
	_out40: cs = 40; goto _out; 
	_out41: cs = 41; goto _out; 
	_out207: cs = 207; goto _out; 
	_out42: cs = 42; goto _out; 
	_out43: cs = 43; goto _out; 
	_out208: cs = 208; goto _out; 
	_out44: cs = 44; goto _out; 
	_out45: cs = 45; goto _out; 
	_out46: cs = 46; goto _out; 
	_out47: cs = 47; goto _out; 
	_out48: cs = 48; goto _out; 
	_out49: cs = 49; goto _out; 
	_out50: cs = 50; goto _out; 
	_out51: cs = 51; goto _out; 
	_out52: cs = 52; goto _out; 
	_out53: cs = 53; goto _out; 
	_out54: cs = 54; goto _out; 
	_out55: cs = 55; goto _out; 
	_out56: cs = 56; goto _out; 
	_out57: cs = 57; goto _out; 
	_out58: cs = 58; goto _out; 
	_out59: cs = 59; goto _out; 
	_out60: cs = 60; goto _out; 
	_out61: cs = 61; goto _out; 
	_out62: cs = 62; goto _out; 
	_out63: cs = 63; goto _out; 
	_out64: cs = 64; goto _out; 
	_out65: cs = 65; goto _out; 
	_out66: cs = 66; goto _out; 
	_out67: cs = 67; goto _out; 
	_out68: cs = 68; goto _out; 
	_out69: cs = 69; goto _out; 
	_out70: cs = 70; goto _out; 
	_out71: cs = 71; goto _out; 
	_out72: cs = 72; goto _out; 
	_out73: cs = 73; goto _out; 
	_out74: cs = 74; goto _out; 
	_out75: cs = 75; goto _out; 
	_out76: cs = 76; goto _out; 
	_out77: cs = 77; goto _out; 
	_out78: cs = 78; goto _out; 
	_out79: cs = 79; goto _out; 
	_out80: cs = 80; goto _out; 
	_out81: cs = 81; goto _out; 
	_out82: cs = 82; goto _out; 
	_out83: cs = 83; goto _out; 
	_out84: cs = 84; goto _out; 
	_out209: cs = 209; goto _out; 
	_out85: cs = 85; goto _out; 
	_out86: cs = 86; goto _out; 
	_out87: cs = 87; goto _out; 
	_out88: cs = 88; goto _out; 
	_out89: cs = 89; goto _out; 
	_out90: cs = 90; goto _out; 
	_out91: cs = 91; goto _out; 
	_out92: cs = 92; goto _out; 
	_out93: cs = 93; goto _out; 
	_out94: cs = 94; goto _out; 
	_out95: cs = 95; goto _out; 
	_out96: cs = 96; goto _out; 
	_out97: cs = 97; goto _out; 
	_out98: cs = 98; goto _out; 
	_out99: cs = 99; goto _out; 
	_out100: cs = 100; goto _out; 
	_out101: cs = 101; goto _out; 
	_out102: cs = 102; goto _out; 
	_out103: cs = 103; goto _out; 
	_out104: cs = 104; goto _out; 
	_out105: cs = 105; goto _out; 
	_out210: cs = 210; goto _out; 
	_out106: cs = 106; goto _out; 
	_out107: cs = 107; goto _out; 
	_out108: cs = 108; goto _out; 
	_out109: cs = 109; goto _out; 
	_out110: cs = 110; goto _out; 
	_out111: cs = 111; goto _out; 
	_out112: cs = 112; goto _out; 
	_out113: cs = 113; goto _out; 
	_out114: cs = 114; goto _out; 
	_out115: cs = 115; goto _out; 
	_out116: cs = 116; goto _out; 
	_out117: cs = 117; goto _out; 
	_out118: cs = 118; goto _out; 
	_out119: cs = 119; goto _out; 
	_out120: cs = 120; goto _out; 
	_out121: cs = 121; goto _out; 
	_out211: cs = 211; goto _out; 
	_out122: cs = 122; goto _out; 
	_out123: cs = 123; goto _out; 
	_out124: cs = 124; goto _out; 
	_out125: cs = 125; goto _out; 
	_out126: cs = 126; goto _out; 
	_out127: cs = 127; goto _out; 
	_out128: cs = 128; goto _out; 
	_out129: cs = 129; goto _out; 
	_out130: cs = 130; goto _out; 
	_out131: cs = 131; goto _out; 
	_out132: cs = 132; goto _out; 
	_out133: cs = 133; goto _out; 
	_out134: cs = 134; goto _out; 
	_out135: cs = 135; goto _out; 
	_out136: cs = 136; goto _out; 
	_out137: cs = 137; goto _out; 
	_out138: cs = 138; goto _out; 
	_out139: cs = 139; goto _out; 
	_out140: cs = 140; goto _out; 
	_out141: cs = 141; goto _out; 
	_out142: cs = 142; goto _out; 
	_out143: cs = 143; goto _out; 
	_out144: cs = 144; goto _out; 
	_out145: cs = 145; goto _out; 
	_out146: cs = 146; goto _out; 
	_out212: cs = 212; goto _out; 
	_out147: cs = 147; goto _out; 
	_out148: cs = 148; goto _out; 
	_out149: cs = 149; goto _out; 
	_out213: cs = 213; goto _out; 
	_out150: cs = 150; goto _out; 
	_out151: cs = 151; goto _out; 
	_out152: cs = 152; goto _out; 
	_out153: cs = 153; goto _out; 
	_out154: cs = 154; goto _out; 
	_out155: cs = 155; goto _out; 
	_out156: cs = 156; goto _out; 
	_out157: cs = 157; goto _out; 
	_out158: cs = 158; goto _out; 
	_out159: cs = 159; goto _out; 
	_out160: cs = 160; goto _out; 
	_out161: cs = 161; goto _out; 
	_out162: cs = 162; goto _out; 
	_out163: cs = 163; goto _out; 
	_out164: cs = 164; goto _out; 
	_out165: cs = 165; goto _out; 
	_out166: cs = 166; goto _out; 
	_out167: cs = 167; goto _out; 
	_out168: cs = 168; goto _out; 
	_out169: cs = 169; goto _out; 
	_out170: cs = 170; goto _out; 
	_out171: cs = 171; goto _out; 
	_out172: cs = 172; goto _out; 
	_out173: cs = 173; goto _out; 
	_out174: cs = 174; goto _out; 
	_out175: cs = 175; goto _out; 
	_out176: cs = 176; goto _out; 
	_out177: cs = 177; goto _out; 
	_out178: cs = 178; goto _out; 
	_out179: cs = 179; goto _out; 
	_out180: cs = 180; goto _out; 
	_out181: cs = 181; goto _out; 
	_out182: cs = 182; goto _out; 
	_out183: cs = 183; goto _out; 
	_out184: cs = 184; goto _out; 
	_out185: cs = 185; goto _out; 
	_out186: cs = 186; goto _out; 
	_out187: cs = 187; goto _out; 
	_out188: cs = 188; goto _out; 
	_out189: cs = 189; goto _out; 
	_out190: cs = 190; goto _out; 
	_out191: cs = 191; goto _out; 
	_out192: cs = 192; goto _out; 
	_out193: cs = 193; goto _out; 
	_out194: cs = 194; goto _out; 
	_out195: cs = 195; goto _out; 
	_out196: cs = 196; goto _out; 
	_out197: cs = 197; goto _out; 
	_out198: cs = 198; goto _out; 
	_out199: cs = 199; goto _out; 
	_out200: cs = 200; goto _out; 
	_out201: cs = 201; goto _out; 
	_out214: cs = 214; goto _out; 
	_out215: cs = 215; goto _out; 
	_out202: cs = 202; goto _out; 
	_out216: cs = 216; goto _out; 
	_out217: cs = 217; goto _out; 
	_out203: cs = 203; goto _out; 
	_out218: cs = 218; goto _out; 
	_out219: cs = 219; goto _out; 

	_out: {}
	}
#line 197 "ext/hpricot_scan/hpricot_scan.rl"
    
    if ( cs == hpricot_scan_error ) {
      free(buf);
      if ( !NIL_P(tag) )
      {
        rb_raise(rb_eHpricotParseError, "parse error on element <%s>, starting on line %d.\n" NO_WAY_SERIOUSLY, RSTRING(tag)->ptr, curline);
      }
      else
      {
        rb_raise(rb_eHpricotParseError, "parse error on line %d.\n" NO_WAY_SERIOUSLY, curline);
      }
    }
    
    if ( done && ele_open )
    {
      ele_open = 0;
      if (tokstart > 0) {
        mark_tag = tokstart;
        tokstart = 0;
        text = 1;
      }
    }

    if ( tokstart == 0 )
    {
      have = 0;
      /* text nodes have no tokstart because each byte is parsed alone */
      if ( mark_tag != NULL && text == 1 )
      {
        if (done)
        {
          if (mark_tag < p-1)
          {
            CAT(tag, p-1);
            ELE(text);
          }
        }
        else
        {
          CAT(tag, p);
        }
      }
      mark_tag = buf;
    }
    else
    {
      have = pe - tokstart;
      memmove( buf, tokstart, have );
      SLIDE(tag);
      SLIDE(akey);
      SLIDE(aval);
      tokend = buf + (tokend - tokstart);
      tokstart = buf;
    }
  }
  free(buf);
}

void Init_hpricot_scan()
{
  VALUE mHpricot = rb_define_module("Hpricot");
  rb_define_attr(rb_singleton_class(mHpricot), "buffer_size", 1, 1);
  rb_define_singleton_method(mHpricot, "scan", hpricot_scan, 1);
  rb_eHpricotParseError = rb_define_class_under(mHpricot, "ParseError", rb_eException);

  s_read = rb_intern("read");
  s_to_str = rb_intern("to_str");
  sym_xmldecl = ID2SYM(rb_intern("xmldecl"));
  sym_doctype = ID2SYM(rb_intern("doctype"));
  sym_procins = ID2SYM(rb_intern("procins"));
  sym_stag = ID2SYM(rb_intern("stag"));
  sym_etag = ID2SYM(rb_intern("etag"));
  sym_emptytag = ID2SYM(rb_intern("emptytag"));
  sym_comment = ID2SYM(rb_intern("comment"));
  sym_cdata = ID2SYM(rb_intern("cdata"));
  sym_text = ID2SYM(rb_intern("text"));
}
