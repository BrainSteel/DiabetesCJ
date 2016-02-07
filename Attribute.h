

/* 
 
    Attribute.h
 
    This file (ab)uses the C Preprocessor to autogenerate code as it pertains to
    attributes. TREAD LIGHTLY.
 
    Last revision 02/06/16 12:02 AM - Jesse Pritchard
 
*/

#ifndef Attribute_h
#define Attribute_h

#include "SDL.h"
#include "Player.h"

/* 
 
 A macro to autogenerate attribute code.
 
 To use:
 
 
*/

#define ATT_GENERATE(X_FUNC) ATT_GENERATE(X_FUNC, ATT_COPY)

#define ATT_GENERATE_MODIFIED(X_FUNC, MODIFIER)                         \
MODIFIER(X_FUNC, Insulin, int, 0, ATT_RENDERBAR)                        \
MODIFIER(X_FUNC, Glucose, int, 0, ATT_RENDERNUM)

/* Some general modifiers of the X_FUNC macros */
#define ATT_COPY(X_FUNC, identifier, type, default, rendertype) X_FUNC(identifier, type, default, rendertype)
#define ATT_POST_COMMA(X_FUNC, identifier, type, default, rendertype) X_FUNC(identifier, type, default, rendertype),
#define ATT_POST_SEMICOLON(X_FUNC, identifier, type, default, rendertype) X_FUNC(identifier, type, default, rendertype);

/* Macros to create usable data from attribute identifiers, types, defaults, and rendertypes */
#define ATT_IDENTIFIER(identifier, type, default, rendertype) identifier
#define ATT_TYPE(identifier, type, default, rendertype) type
#define ATT_DEFAULT(identifier, type, default, rendertype) default
#define ATT_RENDERTYPE(identifier, type, default, rendertype) rendertype

#define ATT_DECLARATION(identifier, type, default, rendertype) type identifier
#define ATT_INITIALIZATION(identifier, type, default, rendertype) identifier = default
#define ATT_ENUM(identifier, type, default, rendertype) ATT_##identifier
#define ATT_NAME(identifier, type, default, rendertype) #identifier
#define ATT_ICONFILE(identifier, type, default, rendertype) (#identifier ".bmp")
#define ATT_UPDATEIDENTIFIER(identifier, type, default, rendertype) ATT_Update##identifier
#define ATT_UPDATEPROTOTYPE(identifier, type, default, rendertype) void ATT_Update##identifier(Player* p)
#define ATT_RENDERIDENTIFIER(identifier, type, default, rendertype) ATT_Render##identifier
#define ATT_RENDERPROTOTYPE(identifier, type, default, rendertype) void ATT_Render##identifier(SDL_Renderer* rend, Player* p)


typedef enum eAttributeType {
    ATT_int,
    ATT_float,
    ATT_other
} AttributeType;

typedef enum eAttributeRender {
    ATT_RENDERBAR,
    ATT_RENDERNUM,
    ATT_RENDEROTHER
} AttributeRender;

typedef enum eAttributeEnum {
    
    ATT_GENERATE_MODIFIED(ATT_ENUM, ATT_POST_COMMA)
    
} AttributeEnum;


typedef struct sAttribute {

    union {
        ATT_GENERATE_MODIFIED(ATT_DECLARATION, ATT_POST_SEMICOLON)
    };
    AttributeEnum type;
    
} Attribute;
#endif /* Attribute_h */
