#include "scanner.h"

// DFA state
typedef enum {
  START,
  INID,
  INNUM,
  INASSIGN,
  INLT,
  INGT,
  INNE,
  INSIGN,
  INCOMMENT,
  SINGLE,
  MULTI,
  STRDATA,
  CHRDATA,
  DONE
} StateType;

// reserved words
static struct {
  char *str;
  TokenType tok;
} reserved_words[MAXRESERVED] = {
    {"if", IF},         {"else", ELSE},   {"switch", SWITCH},  {"case", CASE},
    {"while", WHILE},   {"int", INT},     {"bool", BOOLEAN},   {"char", CHAR},
    {"string", STRING}, {"FALSE", FALSE},    {"TRUE", TRUE},
    {"main", MAIN},     {"print", PRINT}, {"println", PRINTLN}};

// ID -> reserved word
static TokenType reservedLookUp(char *str) {
  for (int i = 0; i < MAXRESERVED; ++i) {
    if (!strcmp(str, reserved_words[i].str)) return reserved_words[i].tok;
  }
  return ID;
}

TokenType getToken() {
  TokenType current_token;
  StateType state = START;
  int token_index = 0;
  int save;  // if save = 1, then save the token
  char c, nc;

  while (state != DONE) {
    c = getc(source);
    if (c == '\n') line_no++;
    save = 1;
    switch (state) {
      case START:
        if (c == EOF) {
          save = 0;
          state = DONE;
          current_token = ENDFILE;
        } else if (isdigit(c)) {
          state = INNUM;
        } else if (isalpha(c)) {
          state = INID;
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
          state = START;
          save = 0;
        } else if (c == '/') {
          state = INCOMMENT;
          save = 0;
        } else if (c == '=') {
          state = INASSIGN;
        } else if (c == '<') {
          state = INLT;
        } else if (c == '>') {
          state = INGT;
        } else if (c == '!') {
          state = INNE;
        } else if (c == '+' || c == '-') {
          state = INSIGN;
          current_token = (c == '+') ? PLUS : MINUS;
        } else if (c == '"') {
          save = 0;
          state = STRDATA;
        } else if (c == '\'') {
          save = 0;
          state = CHRDATA;
        } else {
          state = DONE;
          switch (c) {
            case '(':
              current_token = LPAREN;
              break;
            case ')':
              current_token = RPAREN;
              break;
            case '%':
              current_token = MODULO;
              break;
            case '*':
              current_token = TIMES;
              break;
            case ':':
              current_token = COLON;
              break;
            case ';':
              current_token = SEMI;
              break;
            case ',':
              current_token = COMMA;
              break;
            case '{':
              current_token = LBRACE;
              break;
            case '}':
              current_token = RBRACE;
              break;
            default:
              current_token = OTHER;
              break;
          }
        }
        break;
      case INLT:
        state = DONE;
        if (c == '=') {
          current_token = LE;
        } else {
          save = 0;
          ungetc(c, source);
          current_token = LT;
        }
        break;
      case INGT:
        state = DONE;
        if (c == '=') {
          current_token = GE;
        } else {
          save = 0;
          ungetc(c, source);
          current_token = GT;
        }
        break;
      case INNE:
        state = DONE;
        if (c == '=') {
          current_token = NE;
        } else {
          save = 0;
          ungetc(c, source);
          current_token = NOT;
        }
        break;
      case INSIGN:
        if (!isdigit(c)) {
          save = 0;
          state = DONE;
          ungetc(c, source);
        } else {
          state = INNUM;
        }
        break;
      case INCOMMENT:
        save = 0;
        if (c == '/') {
          state = SINGLE;
        } else if (c == '*') {
          state = MULTI;
        } else {
          state = DONE;
          ungetc(c, source);
          current_token = OVER;
        }
        break;
      case SINGLE:
        save = 0;
        if (c == '\n') {
          state = DONE;
          current_token = COMMENT;
        }
        break;
      case MULTI:
        save = 0;
        if (c == '*') {
          nc = getc(source);
          if (nc == '/') {
            state = DONE;
            current_token = COMMENT;
          } else
            ungetc(nc, source);
        }
        break;
      case INASSIGN:
        state = DONE;
        if (c == '=') {
          current_token = EQ;
        } else {
          save = 0;
          ungetc(c, source);
          current_token = ASSIGN;
        }
        break;
      case INNUM:
        if (!isdigit(c)) {
          save = 0;
          ungetc(c, source);
          state = DONE;
          current_token = NUM;
        }
        break;
      case INID:
        if (!isalnum(c)) {
          save = 0;
          ungetc(c, source);
          state = DONE;
          current_token = ID;
        }
        break;
      case STRDATA:
        if (c == '"') {
          save = 0;
          state = DONE;
          current_token = STRTYPE;
        }
        break;
      case CHRDATA:
        if (c == '\'') {
          save = 0;
          state = DONE;
          current_token = CHARTYPE;
        }
        break;
      default:
        fprintf(stderr, "%s:%d:%d: error: Unexpected state.\n", __FILE__,
                __LINE__, line_no);
        break;
    }
    if (save && token_index < MAXTOKENLEN) token[token_index++] = c;
    if (state == DONE) {
      token[token_index] = '\0';
      if (current_token == ID) current_token = reservedLookUp(token);
    }
  }
  return current_token;
}
/*
int main(int argc, const char *argv[])
{
    char c;
    TokenType t;
    fptr = fopen(argv[1], "r");
    while ((c=getc(fptr)) != EOF) {
      ungetc(c, fptr);
      t = getToken();
      printf("token type %u\n", t);
    }
    return 0;
}
*/
