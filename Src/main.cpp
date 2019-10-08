/*
 * test.cpp
 *
 *  Created on: 19.09.2019
 *      Author: Can
 */





//	HelloWorld.cc

#ifdef __cplusplus

extern "C" {

#endif

#include "main.h"
#include "math.h"
#include "stm32h7xx_hal.h"
#include "test.hpp"
#include "serialize.hpp"



/* Private variables ---------------------------------------------------------*/
#define SWD_SPD 2000000


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void SWD_Init(void);
void activate_cs4272(bool flag);

//const uint32_t arr_size=100000;
//uint32_t __attribute__((section (".bigData"))) mybuf[100000] ;

//SAI handlers
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;

//SPI handlers
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi5;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;

void SystemClock_Config(void);
void SWD_Init(void);
static void MX_GPIO_Init(void);
static void MX_SAI1_Init(void);
static void MX_SPI5_Init(void);
static void MX_SPI3_Init(void);
void init_cs4272(void);
#define buf_size 240


uint32_t __attribute__((section (".dmadata")))  rx_buf[buf_size*4];
uint32_t __attribute__((section (".dmadata")))  tx_buf[buf_size*4];


int main(void){

	HAL_Init();
	SystemClock_Config();
	SWD_Init();
	MX_GPIO_Init();
	MX_SAI1_Init();
	MX_SPI5_Init();
	MX_SPI3_Init();

//	printf("Hallo\n");

	printf("Initializing codec...\t");
	init_cs4272();
	activate_cs4272(1);



	//Dummy data on TX

	uint16_t i;

//
//	int32_t rx_buf[buf_size*4];
//	int32_t tx_buf[buf_size*4];

	for(i=0;i<4*buf_size;i++){

//	tx_buf[i]=(int32_t) 1000*sin((float)i/20);
	tx_buf[i]=0;
//	printf("%d\n",tx_buf[i]);
	};

	//Start DMA Streams
	printf("Initializing SAI DMA Receive stream...\n");
	HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*)&rx_buf, 4*buf_size);
	printf("SAI DMA Receive stream initialized!\n");
	printf("Initializing SAI DMA transmit stream...\n");
	HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*)&tx_buf, 4*buf_size);
	printf("SAI DMA transmit stream initialized!\n");



	construct c;

	  while (1)
	  {
		  c.increment();

		  printf("a: %d \t b: %d\n",c.a,c.b);
		  HAL_Delay(1000);
//		  HAL_GPIO_TogglePin(GPIOB,LD2_Pin);
	  }



//	dyna obj;
//
//	obj.init_array();
//
//
//


	return 0;
}

void dyna::init_array(void){

	unsigned i;

	for(i=0;i<a_size;i++){
		buf[i]=i*2;
	}


}


void construct::increment(void){
	a+=2;
	b+=1;
}


void init_cs4272(void){

	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,(GPIO_PinState)0); //RST low

	uint8_t i=0;
	uint8_t reg[8][3];
	uint8_t adr=0b00100000;

	//Init address
	for(i=0;i<sizeof(reg)/3;i++){
		reg[i][0]=adr;
	}

	//Define startup sequence
	reg[0][1]=0x7; //Power down control
	reg[0][2]=0b00000011; //Set Serial mode + power down

	reg[1][1]=0x1; //Mode control 1
	reg[1][2]=0b00101001; //48kHz Master

	reg[2][1]=0x2;	//DAC Control
	reg[2][2]=0b00000000;

	reg[3][1]=0x3; //DAC Volume control
	reg[3][2]=0b00001001;

	reg[4][1]=0x4;	//DAC Volume A
	reg[4][2]=0b10000000; //No mute + 0dB

	reg[5][1]=0x5;	//DAC Volume B
	reg[5][2]=0b10000000; //No mute + 0dB

	reg[6][1]=0x6;	//ADC Control
	reg[6][2]=0b00010000;

	reg[7][1]=0x7;	//Mode control 2
	reg[7][2]=0b00001010;
//	reg[7][2]=0b00010010;//Loop ADC->DAC


	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1); //MS high: SPI Unselect device
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,(GPIO_PinState)1); //RST High
	HAL_Delay(2); //Wait
	//HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,0); //RST Low

	for(i=0;i<sizeof(reg)/3;i++){
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)0); //MS low: SPI Select device
		HAL_SPI_Transmit(&hspi5,(uint8_t *)&reg[i],3,1000); //Send config register
		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1);//MS high: SPI Unselect device
		HAL_Delay(1); //Wait

	}

}

void activate_cs4272(bool flag){

	uint8_t mutereg[3];
	mutereg[0]=0b00100000;
	mutereg[1]=0x4; //DAC Volume control A register (MUTECA=B is on)

	if(flag){
		mutereg[2]=0b00000000;
	}else{
		mutereg[2]=0b10000000;
	}

	//Send SPI command to CODEC
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)0); //MS low: SPI Select device
	HAL_SPI_Transmit(&hspi5,(uint8_t *)&mutereg,3,1000); //Send config register
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_1,(GPIO_PinState)1);//MS high: SPI Unselect device
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Supply configuration update enable
    */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

    /**Configure the main internal regulator output voltage
    */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
  {

  }
    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI5|RCC_PERIPHCLK_SPI3
                              |RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(SystemCoreClock/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0|GPIO_PIN_1|USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PG0 PG1 USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


/* SAI1 init function */
static void MX_SAI1_Init(void)
{

  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_OUTBLOCKA_ENABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_TX;
  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_32BIT, 2) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

}
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	printf("T\n");
//	tx_status=0;
}


void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){
//	tx_status=1;
//		printf("TXH\n");
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai){

	//	HAL_GPIO_TogglePin(GPIOB,LD2_Pin);
	printf("R\n");
//	rx_status=0;

}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai){
//	rx_status=1;
}

static void MX_SPI3_Init(void)
{

  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  hspi3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI5 init function */
static void MX_SPI5_Init(void)
{

  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT; //16 bit for WM8731
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 7;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
//    _Error_Handler(__FILE__, __LINE__);
  }

}


void SWD_Init(void)
{
  *(volatile uint32_t*)(0x5C001004) |= 0x00700000; // DBGMCU_CR D3DBGCKEN D1DBGCKEN TRACECLKEN

  //UNLOCK FUNNEL
  *(volatile uint32_t*)(0x5C004FB0) = 0xC5ACCE55; // SWTF_LAR
  *(volatile uint32_t*)(0x5C003FB0) = 0xC5ACCE55; // SWO_LAR

  //SWO current output divisor register
  //This divisor value (0x000000C7) corresponds to 400Mhz
  //To change it, you can use the following rule
  // value = (CPU Freq/sw speed )-1

   *(volatile uint32_t*)(0x5C003010) = (SystemCoreClock / SWD_SPD) - 1; // SWO_CODR

  //SWO selected pin protocol register
   *(volatile uint32_t*)(0x5C0030F0) = 0x00000002; // SWO_SPPR

  //Enable ITM input of SWO trace funnel
   *(volatile uint32_t*)(0x5C004000) |= 0x00000001; // SWFT_CTRL

  //RCC_AHB4ENR enable GPIOB clock
   *(volatile uint32_t*)(0x580244E0) |= 0x00000002;

  // Configure GPIOB pin 3 as AF
   *(volatile uint32_t*)(0x58020400) = (*(volatile uint32_t*)(0x58020400) & 0xffffff3f) | 0x00000080;

  // Configure GPIOB pin 3 Speed
   *(volatile uint32_t*)(0x58020408) |= 0x00000080;

  // Force AF0 for GPIOB pin 3
   *(volatile uint32_t*)(0x58020420) &= 0xFFFF0FFF;
}


#ifdef __cplusplus

}

#endif
