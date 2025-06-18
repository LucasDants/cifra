/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * Aplicação: Criptografia ADFGVX
 *
 * Licença: MIT License.
 *
 * Como usar:
 * - Create: Crie os arquivos "message.txt" e "key.txt" no mesmo diretório do código.
 * - Compile Windows: gcc ./cipher_adfgvx.c -o cipher_adfgvx
 * - Compile Mac: clang ./cipher_adfgvx.c -o cipher_adfgvx (pode ser necessário dar permissão para leitura/escrita de arquivos)
 * - Change: Mude os arquivos de entrada conforme o necessário.
 * - Execute: ./cipher_adfgvx
 *
 * Dados de entrada e saída:
 * - Entrada:
 *    Arquivo "./message.txt" contendo a mensagem a ser cifrada em MAIÚSCULAS, podendo haver espaços, vírgula e ponto. Caracteres fora da matriz Polybius serão ignorados.
 *    Arquivo "./key.txt" contendo a chave de transposição (até 8 caracteres).
 * - Saída:
 *    Arquivo "./encrypted.txt" com a mensagem cifrada, onde cada caractere é um símbolo ADFGVX (A, D, F, G, V, X) representando pares de caracteres da matriz Polybius.
 * Autores:
 * - Lucas Dantas
 * - Marcus Vinicius
 *
 * Data: 2025-06
 *
 * Contexto: Trabalho da disciplina de Sistemas Embarcados do IFCE.
 *
 * Plataforma alvo: Microprocessadores com suporte a código C.
 */

#define MAX_MESSAGE_LENGTH 2100
#define KEY_LENGTH 8
#define MAX_PER_COLUMN       ((2*MAX_MESSAGE_LENGTH + KEY_LENGTH - 1) / KEY_LENGTH)

static const char square[6][6] = {
    {'A', 'B', 'C', 'D', 'E', 'F'},
    {'G', 'H', 'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P', 'Q', 'R'},
    {'S', 'T', 'U', 'V', 'W', 'X'},
    {'Y', 'Z', ' ', ',', '.', '1'},
    {'2', '3', '4', '5', '6', '7'}};
static const char symbols[6] = {'A', 'D', 'F', 'G', 'V', 'X'};

static char key[KEY_LENGTH] = "SEMB2025";
static char message[MAX_MESSAGE_LENGTH] = "LOREM IPSUM DOLOR SIT AMET, CONSECTETUR ADIPISCING ELIT. CURABITUR NISI EROS, MAXIMUS A FACILISIS ID, ACCUMSAN NEC TORTOR. MORBI FACILISIS MAGNA SIT AMET TURPIS COMMODO VOLUTPAT. CURABITUR HENDRERIT CURSUS JUSTO, EGET PHARETRA TELLUS VULPUTATE QUIS. PELLENTESQUE ET JUSTO LEO. MAECENAS A EGESTAS ENIM, AC ULTRICES RISUS. UT ET PLACERAT MASSA. LOREM IPSUM DOLOR SIT AMET, CONSECTETUR ADIPISCING ELIT. INTEGER FRINGILLA FINIBUS AUGUE ID SODALES. NULLAM NON FAUCIBUS ANTE. IN PORTTITOR, NIBH ET MATTIS FERMENTUM, VELIT SAPIEN ULLAMCORPER AUGUE, NEC EGESTAS EROS ARCU ID SEM. PELLENTESQUE EU FRINGILLA EX, ID BLANDIT TURPIS. QUISQUE ELIT DOLOR, PORTTITOR A SAPIEN VITAE, MOLESTIE DICTUM TELLUS. SED CONSECTETUR EST NIBH, UT DICTUM EROS EGESTAS SIT AMET. SUSPENDISSE GRAVIDA NEQUE NISL, AT PORTTITOR URNA PORTTITOR ID. NUNC SIT AMET SAPIEN MI. SED POSUERE BLANDIT ENIM AC LUCTUS. PHASELLUS FACILISIS EGET ODIO AC POSUERE. DUIS RUTRUM BIBENDUM ODIO, VITAE VARIUS IPSUM LACINIA A. CRAS QUIS PRETIUM ANTE. DUIS AT AUGUE UT DUI ORNARE MAXIMUS. UT ID LIGULA SED ELIT CONSEQUAT PRETIUM PULVINAR A NISI. PELLENTESQUE DAPIBUS FEUGIAT MAURIS, VEL EGESTAS TORTOR IMPERDIET NON. DONEC TRISTIQUE MASSA NEC EX ELEIFEND VESTIBULUM. VIVAMUS MATTIS SIT AMET VELIT VEL FACILISIS. NULLA FACILISI. DONEC COMMODO QUAM EGET TINCIDUNT HENDRERIT. PROIN MASSA PURUS, CONSECTETUR AC EGESTAS ET, FINIBUS A NEQUE. MAURIS VEL GRAVIDA NISI, ID ELEMENTUM DIAM. SED UT MI LECTUS. AENEAN SCELERISQUE IPSUM MAURIS, NON EUISMOD EST VEHICULA SIT AMET. ALIQUAM NON MAURIS LOREM. NULLA EGESTAS ID MI AC TEMPOR. MORBI A QUAM NON NUNC TEMPUS HENDRERIT. MORBI AT URNA IPSUM. PROIN RHONCUS AUCTOR PURUS AT VESTIBULUM. ETIAM ENIM IPSUM, TEMPUS VEL ELEMENTUM ET, FERMENTUM UT DUI. ETIAM AT QUAM SIT AMET NUNC TEMPUS CONSEQUAT IN ID IPSUM. INTEGER IN TEMPOR LACUS. QUISQUE TINCIDUNT LACINIA ERAT, SED TEMPOR VELIT LOBORTIS IN. PROIN LACINIA DOLOR ANTE, ET ULLAMCORPER ERAT PULVINAR A. MORBI SUSCIPIT DIGNISSIM EROS, UT EFFICITUR DIAM CONVALLIS NEC. INTEGER LAOREET MAURIS VEL TELLUS ELEMENTUM, QUIS PORTA FELIS GRAVIDA. INTEGER LAOREET MAURIS VEL TE";
char encoded_matrix[KEY_LENGTH][MAX_PER_COLUMN];
static int symbols_per_column[KEY_LENGTH] = {0};

/**
 * @brief Encontra os símbolos ADFGVX correspondentes a um caractere.
 *
 * @param c Caractere a ser cifrado.
 * @param row Ponteiro para armazenar o símbolo da linha.
 * @param col Ponteiro para armazenar o símbolo da coluna.
 * @return int Retorna 1 se o caractere foi encontrado, 0 caso contrário.
 */
static int get_adfgvx_symbols(char c, char *row, char *col)
{
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      if (square[i][j] == c)
      {
        *row = symbols[i];
        *col = symbols[j];
        return 1;
      }
    }
  }
  return 0;
}

/**
 * @brief Insere um símbolo ADFGVX na matriz de colunas.
 * @param max_per_column Número máximo de posições em cada coluna
 * @param symbol Símbolo a ser inserido (row ou col).
 * @param symbol_count Contador global de símbolos (será incrementado).
 * @param encoded_symbol_matrix Matriz de saída contendo os símbolos organizados por coluna.
 * @param symbols_per_column Vetor com a quantidade de símbolos por coluna (será atualizado).
 */
static void insert_symbol_to_column(int key_length, int max_per_column, char symbol, int *symbol_count, char encoded_symbol_matrix[KEY_LENGTH][MAX_PER_COLUMN], int symbols_per_column[])
{
  int col_index = (*symbol_count) % key_length;
  int write_pos = symbols_per_column[col_index];

  encoded_symbol_matrix[col_index][write_pos] = symbol;
  symbols_per_column[col_index]++;
  (*symbol_count)++;
}

/**
 * @brief Converte a mensagem em colunas de símbolos ADFGVX para cifra por transposição.
 * @param key_length Comprimento da chave.
 * @param max_per_column Número máximo de posições em cada coluna
 * @param message Mensagem original a ser cifrada.
 * @param encoded_symbol_matrix Matriz onde os símbolos cifrados serão armazenados por coluna.
 * @param symbols_per_column Vetor que armazena o número de elementos em cada coluna.
 */
static void polybius_encode_to_columns(int key_length, int max_per_column, char message[MAX_MESSAGE_LENGTH], char encoded_symbol_matrix[KEY_LENGTH][MAX_PER_COLUMN], int symbols_per_column[])
{
  int i, symbol_count = 0;

  for (i = 0; message[i] != '\0'; i++)
  {
    char row, col;

    // Ignora caracteres que não estão na matriz Polybius
    if (!get_adfgvx_symbols(message[i], &row, &col))
    {
      continue;
    }

    insert_symbol_to_column(key_length, max_per_column, row, &symbol_count, encoded_symbol_matrix, symbols_per_column);
    insert_symbol_to_column(key_length, max_per_column, col, &symbol_count, encoded_symbol_matrix, symbols_per_column);
  }
}

/**
 * @brief Reorganiza as colunas da matriz com base na ordem alfabética da chave.
 * @param key A chave usada na transposição (array de caracteres).
 * @param key_length Comprimento da chave.
 * @param max_per_column Número máximo de posições em cada coluna
 * @param encoded_symbol_matrix Matriz com os dados cifrados por colunas.
 * @param symbols_per_column Vetor com o número de elementos em cada coluna.
 */
static void transpose_columns_by_key_order(char key[], int key_length, int max_per_column, char encoded_symbol_matrix[KEY_LENGTH][MAX_PER_COLUMN], int symbols_per_column[])
{
  int i, j, k;
  char sorted_key[key_length];
  int temp_count;

  // Copia a chave original para preservar sua ordem
  for (i = 0; i < key_length; i++)
  {
    sorted_key[i] = key[i];
  }

  // Ordenação da chave e reorganização das colunas
  for (i = 0; i < key_length - 1; i++)
  {
    for (j = 0; j < key_length - i - 1; j++)
    {
      if (sorted_key[j] > sorted_key[j + 1])
      {
        // Troca os caracteres da chave ordenada
        char tmp = sorted_key[j];
        sorted_key[j] = sorted_key[j + 1];
        sorted_key[j + 1] = tmp;

        // Troca símbolo por símbolo das colunas associadas (até max_per_column)
        for (k = 0; k < max_per_column; k++)
        {
          char temp = encoded_symbol_matrix[j][k];
          encoded_symbol_matrix[j][k] = encoded_symbol_matrix[j + 1][k];
          encoded_symbol_matrix[j + 1][k] = temp;
        }

        // Troca o contador de elementos de cada coluna
        temp_count = symbols_per_column[j];
        symbols_per_column[j] = symbols_per_column[j + 1];
        symbols_per_column[j + 1] = temp_count;
      }
    }
  }
}

/**
 * @brief Aplica a cifra ADFGVX: codifica os símbolos e faz a transposição das colunas.
 * @param key A chave usada na transposição (array de caracteres).
 * @param key_length Comprimento da chave.
 * @param max_per_column Número máximo de posições em cada coluna
 * @param message Mensagem de entrada.
 * @param encoded_symbol_matrix Matriz onde os símbolos codificados serão armazenados.
 * @param symbols_per_column Vetor com a contagem de elementos em cada coluna.
 */
static void cipher_adfgvx(char key[], int key_length, int max_per_column, char message[], char encoded_symbol_matrix[KEY_LENGTH][MAX_PER_COLUMN], int symbols_per_column[])
{
  polybius_encode_to_columns(key_length, max_per_column, message, encoded_symbol_matrix, symbols_per_column);
  transpose_columns_by_key_order(key, key_length, max_per_column, encoded_symbol_matrix, symbols_per_column);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  while (1)
//  {
    cipher_adfgvx(key, KEY_LENGTH, MAX_PER_COLUMN, message, encoded_matrix, symbols_per_column);

    // Print symbols column by column, in key-sorted order
    for (int i = 0; i < KEY_LENGTH; i++)
    {
      for (int j = 0; j < symbols_per_column[i]; j++)
      {
        char c = encoded_matrix[i][j];
        HAL_UART_Transmit(&huart2, (uint8_t *)&c, 1, HAL_MAX_DELAY);
      }
    }
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
//  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
