#include <stdbool.h>

#define EOL         -1
#define OVERFLOW    -2
#define NO_NUMBER   -3


/*---------------------------------------------------------------------------------------
 *
 * Funções equivalentes às existentes no <ctype.h> 
 * 
 * --------------------------------------------------------------------------------------*/
bool char_is_lower (char c);

bool char_is_upper (char c);

bool char_is_alpha (char c);

bool char_is_digit (char c);

bool char_is_alpha_num (char c);

bool char_is_blank (char c);

int char_to_upper (char c);

int char_to_lower (char c);


int str_lenght (const char str[]);

int str_is_empty (const char str[]);

/**
 * Salta brancos inciais
 */
void str_skip_spaces (const char str[], int *pos);

void str_skip_spaces_from_end (const char str[], int *pos);

/**
 * Elimina da string todos os caracteres considerando que o simbolo commenChar 
 * representa um comentário de linha
 * 
 */
void str_trim_comments (char str[], char commentChar);


 /* 
  * Retorna:
  *   0 em caso de sucesso
  *   EOL (-1) se não foi encontrado nenhum digito.
  *   OVERFLOW (-2) se o número é demasiado grande para caber num inteiro.
  */ 
int str_get_int (const char str[], int *num);


int str_is_at_end (const char line[], int *pos);




/*---------------------------------------------------------------------------------------
 *
 * A realizar no Ex.1 do T2 
 *
 *---------------------------------------------------------------------------------------*/

/**
 * Converte todas as letra da string "str" para minúsculas
 */
void str_to_lower (char str[]);  


/**
 * Converte todas as letra da string "str" para maíusculas
 */  
void str_to_upper (char str[]);    


/**
 * Retira os separadores iniciais e finais da string "str"
 * Os separadores podem ser espaço e/ou tab
 */
void str_trim (char str[]);       




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
 *   OVERFLOW (-2)  se a sequência foi truncada por ser maior ou igual a max_size.
 * 
 */
int str_next_word (const char line[], int *pos, char word[], int max_size);	  


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
int str_next_int (const char line[], int *pos, int *num);  
	 


 

    
