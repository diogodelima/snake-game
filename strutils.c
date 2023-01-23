#include "strutils.h"

#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>


/*---------------------------------------------------------------------------------------
 *
 * Funções equivalentes às existentes no <ctype.h> 
 * 
 * --------------------------------------------------------------------------------------*/

bool char_is_lower (char c)
{
    return (c >= 'a' && c <= 'z');
}

bool char_is_upper (char c)
{
    return (c >= 'A' && c <= 'Z');
}

bool char_is_alpha (char c)
{
    return char_is_lower(c) || char_is_upper(c);
}

bool char_is_digit (char c)
{
    return (c >= '0' && c <= '9');
}

bool char_is_alpha_num (char c)
{
    return char_is_alpha(c) || char_is_digit(c);
}

/**
 *  Idêntica ao isspace().
 *  In  the "C" and "POSIX" locales, these are: space, form-feed ('\f'), newline ('\n'), 
 *  carriage return ('\r'), horizontal tab ('\t'), and vertical tab ('\v').
 */
bool char_is_space (char c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}


int char_to_upper (char c)
{
    if ( char_is_lower(c) ) return c - ('a' - 'A');
    return c;
}

int char_to_lower (char c)
{
    if ( char_is_upper(c) ) return c + ('a' - 'A');
    return c;
}

 
int str_lenght (const char str[])
{
    int i = 0;

    while (str[i] != '\0') i++;

    return i;
}

int str_is_empty (const char str[])
{
    return str[0] == '\0';
}


/**
 * Salta brancos inciais
 */
void str_skip_spaces (const char str[], int *pos)
{
    while (char_is_space(str[*pos])) 
        (*pos)++;
}

void str_skip_spaces_from_end (const char str[], int *pos)
{
    while (char_is_space(str[*pos])) 
        (*pos)--;
}

void str_trim_comments (char str[], char commentChar)
{
    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] == commentChar) {
            str[i] = '\0';
        }
    }
}

 /**
  * Retorna:
  *   0 em caso de sucesso
  *   NO_NUMBER (-3) se não foi encontrado nenhum número.
  *   OVERFLOW (-2) se o número é demasiado grande para caber num inteiro.
  */ 
int str_get_int (const char str[], int *num) 
{
    long value  = 0;
    int  idx    = 0;
    int  signal = 1;;

    if (str[idx] == '+') {
        idx++;
    }
    else if (str[idx] == '-') {
        idx++;
        signal = -1;
    }
    
    // se não for um digito devolve erro
    if ( !char_is_digit(str[idx]) ) return NO_NUMBER;

    while ( str[idx] != '\0' && char_is_digit(str[idx])) {
        value = value * 10 + str[idx] - '0';
        idx++;
    }
    
    // inteiro termina com um separador
    if ( str[idx] != '\0' && !char_is_space(str[idx]) ) return NO_NUMBER;

    value = value * signal;
    
    if (value < INT_MIN || value > INT_MAX) return OVERFLOW;

    *num = value;
    return 0;
}


int str_is_at_end (const char line[], int *pos) 
{
    return line[*pos] == '\0';
}



/*---------------------------------------------------------------------------------------
 *
 * A realizar no Ex.1 do T2 
 *
 *---------------------------------------------------------------------------------------*/

/**
 * Converte todas as letra da string "str" para minúsculas
 */
void str_to_lower(char str[]) 
{
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = char_to_lower(str[i]);
    }
}


/**
 * Converte todas as letra da string "str" para maíusculas
 */  
void str_to_upper(char str[]) 
{
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = char_to_upper(str[i]);
    }
}


/**
 * Retira os separadores iniciais e finais da string "str"
 * Os separadores podem ser espaço e/ou tab
 */
void str_trim (char str[]) 
{
    int lenght = str_lenght(str);

    // se string vazia
    if (lenght == 0) return;

    // pecorre a partir do inicio enquanto separador
    int first = 0;
    str_skip_spaces(str, &first);

    int last = lenght -1;
    if (last > first) {
        str_skip_spaces_from_end(str, &last);
    }
    
    // Desloca string para o início 
    if (first > 0) {
        int dst = 0;
        for (int src = first; src <= last; src++) {
            str[dst] = str[src];
            dst++;
        }
    }

    // atualiza terminador da string
    str[last - first + 1] = '\0';
/*
    int lenght = str_lenght(str);

    // se string vazia
    if (lenght == 0) return;

    // pecorre a partir do inicio enquanto separador
    int first = 0;
    while (char_is_space(str[first])) first++;

    int last = lenght -1;
    if (last > first) {
        while (char_is_space(str[last])) last--;
    }
    
    // Desloca string para o início 
    if (first > 0) {
        int dst = 0;
        for (int src = first; src <= last; src++) {
            str[dst] = str[src];
            dst++;
        }
    }

    // atualiza terminador da string
    str[last - first + 1] = '\0';
*/
}



/**
 * Coloca em "word" a próxima sequência de caracteres "não espaços" em "line",
 * de dimensão máxima "max_size"-1,  a partir da posição indicada por "pos",
 * saltando eventuais espaços iniciais.
 * O valor referido por "pos" é modificado para a posição do caracter imediatamente 
 * a seguir à sequeência retornada.
 * 
 * Retorna:
 *   0 em caso de sucesso
 *   EOL (-1) se não foi encontrado nenhum caracter "não espaço" até ao fim da string.
 *   OVERFLOW (-2)  se a sequência foi truncada por ser maior ou igual a capacity.
 * 
 */
int str_next_word (const char line[], int *pos, char word[], int max_size) 
{
	int src = *pos;
    
    str_skip_spaces(line, &src);
    if (line[src] == '\0') {
        *pos = src;
        return EOL;
    }

    int dst = 0;
    while ( dst < max_size-1 && line[src] != '\0' && ! char_is_space(line[src])) {
        word[dst++] = line[src++];
    }

    word[dst] = '\0';
    *pos = src;
    
    if (line[src] != '\0' && ! char_is_space(line[src])) return OVERFLOW;

    return 0;
}


/**
 * Afeta o inteiro referido por "num", com o valor obtido da sequência  
 * de dígitos em "line", a partir da posição indicada por "pos",
 * saltando eventuais espaços iniciais.
 * O valor referido por "pos" é modificado para a posição do caracter imediatamente 
 * a seguir à sequència de dígitos correspondente ao inteiro obtido.
 * 
 * Retorna:
 *   0 em caso de sucesso
 *   NO_NUMBER (-3) se não foi encontrado nenhum número.
 *   OVERFLOW (-2) se o número é demasiado grande para caber num inteiro.
 * 
 */
int str_next_int (const char line[], int *pos, int *num) 
{
	char word[12];
    
    int res = str_next_word(line, pos, word, 12);
    if (res < 0) return res;

    return str_get_int(word, num);
}

int str_next_int_v0 (const char line[], int *pos, int *num) 
{
	int src = *pos;
    
    str_skip_spaces(line, &src);
    if (line[src] == '\0') {
        *pos = src;
        return EOL;
    }

    long value = 0;

    int signal = 1;
    if (line[src] == '+') src++;
    else if (line[src] == '-') {
        src++;
        signal = -1;
    }
    
    while ( line[src] != '\0' && char_is_digit(line[src])) {
        value = value * 10 + line[src] - '0';
        src++;
    }
    value = value * signal;

    *pos = src;
    
    if (value < INT_MIN || value > INT_MAX) return OVERFLOW;

    *num = value;
    return 0;
}
