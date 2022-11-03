#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//#define SHUFFLE_SYMBOLS_ON_CARD
#define CHECK_IF_DATA_VALID
#define CHECK_IF_UNIQUE_SYMBOL_ON_CARDS
#define DISPLAY_OUTPUT_NUMERIC_DATA
#define DISPLAY_OUTPUT_TXT_DATA

#define NUMBER_SYMBOLS_ON_CARD 8
#define PRIME_NUMBER (NUMBER_SYMBOLS_ON_CARD - 1)
#define NUMBER_OF_CARDS_TO_GENERATE ((PRIME_NUMBER * PRIME_NUMBER) + PRIME_NUMBER + 1)
#define MAX_STRING_SIZE 24

char symbols_array[][MAX_STRING_SIZE] = {
  "Anchor", "Apple", "Bomb", "Cactus", "Candle",
  "Carrot", "Cheese", "Chess knight", "Clock", "Clown",
  "Diasy flower","Dinosaur", "Dolphin", "Dragon", "Exclamation mark",
  "Eye", "Fire", "Four leaf clover", "Ghost", "Green splats",
  "Hammer", "Heart", "Ice cube", "Igloo", "Key",
  "Ladybird", "Light bulb", "Lightning bolt", "Lock", "Maple leaf",
  "Milk bottle", "Moon", "No Entry sign", "Orange scarecrow man", "Pencil",
  "Purple bird", "Purple cat", "Purple dobble sign", "Question Mark", "Red lips",
  "Scissors", "Skull and crossbones", "Snowflake", "Snowman", "Spider",
  "Spider’s web", "Sun", "Sunglasses", "Target", "Taxi",
  "Tortoise", "Treble clef", "Tree", "Water drop", "Dog",
  "Yin and Yang", "Zebra"
};

//Tablica będzie przechowywała tylko wartości numeryczne odpowiadające pozycji
//nazwy podanego obrazu z tablicy symbols_array
uint8_t cards[NUMBER_OF_CARDS_TO_GENERATE][NUMBER_SYMBOLS_ON_CARD];

int main(void)
{
    uint8_t cards_tmp[NUMBER_OF_CARDS_TO_GENERATE * NUMBER_SYMBOLS_ON_CARD] = { 0x00 };
    uint16_t pos_in_tmp_buffer = 0;

    printf("\r\n#######################################################\r\n\r\n");
    printf("Liczba symboli na karcie - %d\r\n", NUMBER_SYMBOLS_ON_CARD);
    printf("Liczba kart do wygenerowania - %d\r\n", NUMBER_OF_CARDS_TO_GENERATE);
    printf("Prime number - %d\r\n", PRIME_NUMBER);
    printf("\r\n#######################################################\r\n\r\n");

    //Przygotowanie pierwszych kart
    for (uint8_t i = 0; i < (PRIME_NUMBER + 1); i++)
    {
        cards[i][0] = 1;
        for (uint8_t j = 1; j < (NUMBER_SYMBOLS_ON_CARD); j++)
        {
            cards[i][j] = ((j) + (i*PRIME_NUMBER) + 1);
        }
    }

    //Przygotowanie reszty kart
    for (uint8_t i = 0; i < PRIME_NUMBER; i++) {
        for (uint8_t j = 0; j < PRIME_NUMBER; j++) {
            cards_tmp[pos_in_tmp_buffer++] = i + 2;
            for (uint8_t k = 0; k < PRIME_NUMBER; k++) {
                cards_tmp[pos_in_tmp_buffer++] = (PRIME_NUMBER + 1 + PRIME_NUMBER * k + (i*k + j) % PRIME_NUMBER) + 1;
            }
        }
    }

    //Przepisanie kart z bufora tymczasowego do bufora głównego:
    for (uint16_t i = NUMBER_SYMBOLS_ON_CARD; i < NUMBER_OF_CARDS_TO_GENERATE; i++) {
        for (uint8_t j = 0; j < NUMBER_SYMBOLS_ON_CARD; j++) {
            cards[i][j] = cards_tmp[((i - NUMBER_SYMBOLS_ON_CARD) * NUMBER_SYMBOLS_ON_CARD) + j];
        }
    }


    #ifdef SHUFFLE_SYMBOLS_ON_CARD
    uint8_t shuffle_tmp_array = 0;

    for (uint8_t cardNumber = 0; cardNumber < NUMBER_OF_CARDS_TO_GENERATE; cardNumber++)
    {
        for (uint8_t i = 0; i < NUMBER_SYMBOLS_ON_CARD; i++)
        {
            uint8_t j = i + rand() / (RAND_MAX / (NUMBER_SYMBOLS_ON_CARD - i) + 1);
            uint8_t t = cards[cardNumber][j];
            cards[cardNumber][j] = cards[cardNumber][i];
            cards[cardNumber][i] = t;
        }
    }
    #endif

    #ifdef CHECK_IF_UNIQUE_SYMBOL_ON_CARDS
    for (uint8_t cardNumber = 0; (cardNumber < (NUMBER_OF_CARDS_TO_GENERATE)); cardNumber++) {
        for (int i = 0; i < NUMBER_SYMBOLS_ON_CARD - 1; i++) {
            for (int j = i + 1; j < NUMBER_SYMBOLS_ON_CARD; j++) {
                if (cards[cardNumber][i] == cards[cardNumber][j]) {
                    printf("\r\n#######################################################\r\n\r\n");
                    printf("ERROR Ten sam symbol na karcie %d co %d\r\n", cardNumber, cardNumber);
                    printf("\r\n#######################################################\r\n\r\n");
                    return 0;
                }
            }
        }
    }
    printf("## Kazda karta zawiera unikalne symbole\r\n\r\n");
    #endif

    #ifdef CHECK_IF_DATA_VALID
    //Sprawdzenie poprawności
    uint8_t match_card_status = 0;
    for (uint8_t cardNumber = 0; (cardNumber < (NUMBER_OF_CARDS_TO_GENERATE - 1)); cardNumber++)
    {
        for (uint8_t cardNumber_Second = cardNumber + 1; cardNumber_Second < NUMBER_OF_CARDS_TO_GENERATE; cardNumber_Second++)
        {
            for (uint8_t symbolNumber_1 = 0; symbolNumber_1 < NUMBER_SYMBOLS_ON_CARD; symbolNumber_1++)
            {
                for (uint8_t symbolNumber_2 = 0; symbolNumber_2 < NUMBER_SYMBOLS_ON_CARD; symbolNumber_2++)
                {
                    if (cards[cardNumber][symbolNumber_1] == cards[cardNumber_Second][symbolNumber_2])
                    {
                        if (match_card_status == 1) {
                            printf("ERROR karta %d symbol %d z %d %d !!!!\r\n", cardNumber, symbolNumber_1, cardNumber_Second, symbolNumber_2);
                        }
                        match_card_status = 1;
                    }
                }
            }
            if (match_card_status == 0) {
                printf("ERROR brak dopasowania symbolu pomiedzy kartami %d z %d!!!!\r\n", cardNumber, cardNumber_Second);
            }
            match_card_status = 0;
        }
    }
    #endif

    #ifdef DISPLAY_OUTPUT_NUMERIC_DATA
    //Wyświetlenie wartości numerycznej
    for (uint8_t cardNumber = 0; cardNumber < NUMBER_OF_CARDS_TO_GENERATE; cardNumber++)
    {
        printf("Karta %.2u - ", (cardNumber + 1));
        for (uint8_t symbolNumber = 0; symbolNumber < NUMBER_SYMBOLS_ON_CARD; symbolNumber++)
        {
            printf("%.2u, ", cards[cardNumber][symbolNumber]);
        }
        printf("\r\n");
    }
    #endif

    #ifdef DISPLAY_OUTPUT_TXT_DATA
    printf("\r\n#######################################################\r\n\r\n");
    //Wyświetlenie znaku przypisanego do wartości
    for (uint8_t cardNumber = 0; cardNumber < NUMBER_OF_CARDS_TO_GENERATE; cardNumber++)
    {
        printf("Karta %.2u - ", (cardNumber + 1));
        for (uint8_t symbolNumber = 0; symbolNumber < NUMBER_SYMBOLS_ON_CARD; symbolNumber++)
        {
            printf("%s, ", symbols_array[cards[cardNumber][symbolNumber] - 1]);
        }
        printf("\r\n");
    }
    #endif

    return 0;
}
