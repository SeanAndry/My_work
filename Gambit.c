#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef enum{ two, three, four, five, six, seven, eight, nine, ten, jack, queen, king,
    ace } faces_t;
typedef enum{ spades, clubs, diamonds, hearts } suit_t;
typedef enum{ FALSE, TRUE } bool_t;
typedef enum{none, pair, two_pair, three_kind, straight, flush, 
full_house, four_of_kind, straight_flush, royal_flush} ranking_t;

typedef struct
{
  faces_t face;
  suit_t suit;
  bool_t pulled;
  bool_t valid;
} card_t;

#define HAND_SZ 7
#define NUM_CARDS_IN_DECK 52
static card_t *pull_a_card (void);
static card_t deck[NUM_CARDS_IN_DECK];
static void initialize_deck (void);
static bool_t is_straight(void);
static bool_t is_flush(void);
static ranking_t is_duplicate(void);
static void set_pulled(bool_t state, size_t len, card_t* cards);
static void set_valid(bool_t state, size_t len, card_t* cards);

const char *suit_strings[] = {"spades","clubs","diamonds","hearts"};
const char *face_strings[] = { "two", "three", "four", "five", "six",
    "seven", "eight", "nine", "ten", "jack", "queen", "king", "ace" };
const char *ranking_strings[] = { "none", "pair", "two_pair", "three_kind", "straight", "flush", 
"full_house", "four_of_kind", "straight_flush", "royal_flush"};
static card_t my_hand[HAND_SZ];
static int num_remaining = NUM_CARDS_IN_DECK;

int main ()
{
    int num_cards = 0;
    card_t *a_card;
    initialize_deck ();
    srand (time(NULL));
  
  for (size_t i = 0; i < HAND_SZ; i++)
    {
    a_card = pull_a_card ();
    // Assign (copy) the suit and face values
    // from the card in the deck to the card
    // in my hand.
    my_hand[i].suit = a_card->suit;
    my_hand[i].face = a_card->face;
    my_hand[i].pulled = a_card->pulled;
    }

  // print the cards in our hand
  for (size_t i = 0; i < HAND_SZ; i++)
    {
    printf ("Card number %u is the %s of %s\n",
	      (unsigned int) (i + 1),
	      face_strings[my_hand[i].face],
	      suit_strings[my_hand[i].suit]);
    }

    printf("The hand is %sa straight\n", is_straight() == TRUE ? "" : "not ");
    printf("The hand is %sa flush\n", is_flush() == TRUE ? "" : "not ");
    printf("You have a %s\n", ranking_strings[is_duplicate()]);
    return 0;  
}

void initialize_deck (void)
{
  size_t index = 0;
  do
    {
      for (suit_t s = spades; s <= hearts; s++)
    	{
    	  for (faces_t f = two; f <= ace; f++)
    	    {
    	      deck[index].suit = s;
    	      deck[index].face = f;
    	      deck[index].pulled = FALSE;
    	      deck[index].valid = TRUE;
    	      index++;
    	    }
    	}
    }
  while (index < NUM_CARDS_IN_DECK);
}


card_t * address_of_card (suit_t suit, faces_t face)
{
  size_t index = 0;
  do
    {
      if (deck[index].suit == suit && deck[index].face == face)
    	{
    	    
    	  return &deck[index];
    	}
          index++;
        }
  while (index < NUM_CARDS_IN_DECK);
  return NULL;
}

suit_t random_suit ()
{
  return rand () % 4;
}

faces_t random_face ()
{
  return rand () % 13;
}

card_t * pull_a_card (void)
{
    if(num_remaining == 0)
    {
        return NULL;
    }
    int card_picked = (rand() % num_remaining);
    for(int index = 0; index < NUM_CARDS_IN_DECK; index++)
    {
        if(deck[index].pulled)
        {
            continue;
        }
        if (card_picked-- == 0)
        {
            deck[index].pulled = TRUE;
            deck[index].valid = TRUE;
            return &deck[index];
        }
    }
    return NULL;
}

void set_pulled(bool_t state, size_t len, card_t* cards )
{
    while(len--)
    {
        if (cards)
        {
            cards->pulled = state;
            cards++;
        }
    }
}

void set_valid(bool_t state, size_t len, card_t* cards )
{
    while(len--)
    {
        if (cards)
        {
            cards->valid = state;
            cards++;
        }
    }
}
//********checks for card combinations********

bool_t is_flush(void)
{
    suit_t s = my_hand[0].suit;
    for(int index = 1; index < HAND_SZ; index++)
    {
        if(my_hand[0].suit != my_hand[index].suit)
        {
            return FALSE;
        }
    }
    return TRUE;
}

    //loop through each rank
    //for each position in my ordered hand
    //loop through each card in my hand, looking for the current rank.
    //if there are no cards matching the rank then break out of loop and go to next rank
    //if source hand card matches current rank then set the current position of the ordered hand
    //to this card, then move to the next index in the ordered hand
bool_t is_straight(void)
{
    faces_t compare_face = ace;
    card_t* ordered_hand[HAND_SZ];
    for (int i = 0; i < HAND_SZ;)//loop to index through each position in my ordered hand
    {
        for (int j = 0; j < HAND_SZ; j++)//looping through the hand
        {
            //if the card in the hand matches the possible face value
            //the ordered hand array is updated with the card from the hand
            //that matches the possible face value
            
            if (my_hand[j].face == compare_face && my_hand[j].pulled == TRUE)
            {
                ordered_hand[i] = &my_hand[j];
                i++;
                my_hand[j].pulled = FALSE;
                break;
            }
            else 
            {
                //are we at the last card?
                if (j == (HAND_SZ-1))
                {
                    compare_face--;
                }
            }
        }
    }
    compare_face = ordered_hand[0]->face;
    for(int k = 0; k < HAND_SZ; k++)
    {
        if(ordered_hand[k]->face != compare_face--)
        {
            return FALSE;
        }
    }
    return TRUE;
}

ranking_t is_duplicate(void)
{
    ranking_t best_hand = none;
    set_pulled(FALSE, HAND_SZ, &my_hand[0]);
    set_valid(TRUE, HAND_SZ, &my_hand[0]);
    for(int i = 0; i < HAND_SZ; i++)//loop to index through each position in my hand
    {
        if(my_hand[i].valid == TRUE)//only pulls and compares the card if it is valid
        {
            faces_t compare_face = my_hand[i].face;//sets the first card in array to compare to rest of cards
            my_hand[i].valid = FALSE;
            my_hand[i].pulled = TRUE;
            for(int j = i+1; j < HAND_SZ; j++)//loops through the still availble cards to compare
            {
                //if compare face is equal to a valid card in the hand, pull it
                if(compare_face == my_hand[j].face && my_hand[j].valid == TRUE)
                {
                    my_hand[j].pulled = TRUE;
                }
            }
        }
        int num_duplicates = 0; //initializes the number of pulled cards to 0, reset every time
        //if there are duplicates this will be incremented
        
        for(int k = 0; k < HAND_SZ; k++)//loops through and finds the number of pulled cards
        { 
            if(my_hand[k].pulled == TRUE && my_hand[k].valid == TRUE)
            {
                my_hand[k].valid = FALSE;
                num_duplicates++;
            }
        }
        switch(num_duplicates)
        {
        case 0 :
            break;
        case 1 ://pair
            if(best_hand < pair)
                best_hand = pair;
            else if(best_hand == pair)
                best_hand = two_pair;
            else if(best_hand == three_kind)
                best_hand == full_house;
            break;
        case 2 ://triple
            if(best_hand == pair)
                best_hand = full_house;
            else if(best_hand < two_pair)
                best_hand = three_kind;
            break;
        case 3 ://four of a kind
            if(best_hand < full_house)
                best_hand = four_of_kind;
            break;
        default :
        printf("Invalid\n" );
       }
    }
    return best_hand;
}


