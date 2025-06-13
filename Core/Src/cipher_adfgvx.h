#ifndef CIPHER_ADFGVX_H
#define CIPHER_ADFGVX_H

#define MAX_MESSAGE_LENGTH 2560
#define MAX_KEY_LENGTH 9

extern const char symbols[6];
extern const char square[6][6];

/**
 * @brief Encontra os símbolos ADFGVX correspondentes a um caractere.
 *
 * @param c Caractere de entrada.
 * @param row Ponteiro para armazenar o símbolo da linha.
 * @param col Ponteiro para armazenar o símbolo da coluna.
 * @return int 1 se o caractere foi encontrado, 0 caso contrário.
 */
int get_adfgvx_symbols(char c, char *row, char *col);

/**
 * @brief Insere um símbolo ADFGVX em colunas para transposição.
 *
 * @param key_length Comprimento da chave.
 * @param max_per_column Número máximo de posições por coluna.
 * @param symbol Símbolo ADFGVX a ser inserido.
 * @param symbol_count Contador de símbolos total (referência).
 * @param encoded_symbol_matrix Matriz de símbolos por coluna.
 * @param symbols_per_column Vetor com a quantidade de símbolos por coluna.
 */
void insert_symbol_to_column(int key_length, int max_per_column, char symbol, int *symbol_count, char encoded_symbol_matrix[key_length][max_per_column], int symbols_per_column[]);

/**
 * @brief Codifica a mensagem com a matriz Polybius e distribui em colunas.
 *
 * @param key_length Comprimento da chave.
 * @param max_per_column Tamanho máximo por coluna.
 * @param message Mensagem original.
 * @param encoded_symbol_matrix Matriz de símbolos codificados.
 * @param symbols_per_column Quantidade de símbolos por coluna.
 */
void polybius_encode_to_columns(int key_length, int max_per_column, int message_length, char message[message_length], char encoded_symbol_matrix[key_length][max_per_column], int symbols_per_column[]);

/**
 * @brief Reorganiza as colunas com base na ordem alfabética da chave.
 *
 * @param key A chave usada na transposição.
 * @param key_length Tamanho da chave.
 * @param max_per_column Máximo de posições por coluna.
 * @param encoded_symbol_matrix Matriz de símbolos organizados por coluna.
 * @param symbols_per_column Número de símbolos em cada coluna.
 */
void transpose_columns_by_key_order(char key[], int key_length, int max_per_column, char encoded_symbol_matrix[key_length][max_per_column], int symbols_per_column[]);

/**
 * @brief Aplica a cifra completa ADFGVX: Polybius + transposição.
 *
 * @param key A chave de transposição.
 * @param key_length Comprimento da chave.
 * @param max_per_column Máximo de símbolos por coluna.
 * @param message A mensagem original.
 * @param encoded_symbol_matrix Matriz de saída codificada.
 * @param symbols_per_column Vetor com contagem de símbolos por coluna.
 */
void cipher_adfgvx(char key[], int key_length, int max_per_column, char message[], int message_length, char encoded_symbol_matrix[key_length][max_per_column], int symbols_per_column[]);

#endif // CIPHER_ADFGVX_H
