#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "poker.h"

/* converts a hand (of 5 cards) to a string representation, and stores it in the
 * provided buffer. The buffer is assumed to be large enough.
 */
void hand_to_string (hand_t hand, char *handstr) {
    char *p = handstr;
    int i;
    char *val, *suit;
    for (i=0; i<5; i++) {
        if (hand[i].value < 10) {
            *p++ = hand[i].value + '0';
        } else {
            switch(hand[i].value) {
            case 10: *p++ = 'T'; break;
            case 11: *p++ = 'J'; break;
            case 12: *p++ = 'Q'; break;
            case 13: *p++ = 'K'; break;
            case 14: *p++ = 'A'; break;
            }
        }
        switch(hand[i].suit) {
        case DIAMOND: *p++ = 'D'; break;
        case CLUB: *p++ = 'C'; break;
        case HEART: *p++ = 'H'; break;
        case SPADE: *p++ = 'S'; break;
        }
        if (i<=3) *p++ = ' ';
    }
    *p = '\0';
}

/* converts a string representation of a hand into 5 separate card structs. The
 * given array of cards is populated with the card values.
 */
void string_to_hand (const char *handstr, hand_t hand) {
     const char *p = handstr;
     int i;
     for (i = 0; i < 5; i++) {
	     switch(handstr[i * 3]) {
	       case '2': hand[i].value = '2' - '0'  ; break;
               case '3': hand[i].value = '3' - '0'  ; break;
	       case '4': hand[i].value = '4' - '0' ; break;
               case '5': hand[i].value = '5' - '0' ; break;
               case '6': hand[i].value = '6' - '0' ; break;		
	       case '7': hand[i].value = '7' - '0' ; break;
	       case '8': hand[i].value = '8' - '0' ; break;
	       case '9': hand[i].value = '9' - '0' ; break;
	       case 'T': hand[i].value = 10 ; break;
               case 'J': hand[i].value = 11 ; break;
	       case 'Q': hand[i].value = 12 ; break;
               case 'K': hand[i].value = 13 ; break;
               case 'A': hand[i].value = 14 ; break;
	     }
		switch(handstr[1 + (i*3)]) {
        	case 'D': hand[i].suit  = DIAMOND ; break;
        	case 'C': hand[i].suit  = CLUB    ; break;
        	case 'H': hand[i].suit  = HEART   ; break;
        	case 'S': hand[i].suit  = SPADE   ; break;
             }
 }

}

/* sorts the hands so that the cards are in ascending order of value (two
 * lowest, ace highest */
void sort_hand (hand_t hand) {
     int i, k;
     card_t temp;
     int index;
     int subArrayLength;
     int arraySize = 5;
     for (i = 0; i < arraySize ; i++) {
	     subArrayLength = arraySize - i;
	     index = 0;
	     for (k = 1; k < subArrayLength; k++) {
		     if (hand[k].value > hand[index].value) {
			     index = k;
			}
	     }
		temp = hand[index]; 
		hand[index] = hand[arraySize - i - 1];
		hand[arraySize - i - 1] = temp;
	     
}
}

int count_pairs (hand_t hand) {
  int i;
  int numPairs = 0;
  for (i = 0; i < 4; i ++) {
	  if (hand[0].value == hand[i + 1].value) {
		 numPairs++;
		 break; 
	  }
  }
  i = 0;
 for (i = 1; i < 4; i ++) {
	  if (hand[1].value == hand[i + 1].value) {
		 numPairs++;
		 break; 
	  }
  }
  i = 0;
 for (i = 2; i < 4; i ++) {
	  if (hand[2].value == hand[i + 1].value) {
		 numPairs++;
		 break; 
	  }
  }
  i = 0;
 for (i = 3; i < 4; i ++) {
	  if (hand[3].value == hand[i + 1].value) {
		 numPairs++;
		 break; 
	  }
  }
    return numPairs;
}

int is_onepair (hand_t hand) {
    
    int numPairs = 0;
    int pairs = count_pairs(hand);
    if (pairs >= 1) {
	    numPairs = 1;
    return numPairs;
    }
    else
    return numPairs; 

}

int is_twopairs (hand_t hand) {
    
    int numPairs = 0;
    int pairs = count_pairs(hand);
    if (pairs == 2) {
	    numPairs = 1;
    return numPairs;
    }
    else
    return numPairs; 

}

int is_threeofakind (hand_t hand) {
     int i;
     int threeOfAKind = 0;
     sort_hand(hand);
     for (i = 0; i < 3; i++) {
	  if ( (hand[0].value == hand[i + 1].value) && (hand[i + 1].value == hand[i + 2].value) ) {
		 threeOfAKind++; 
		 break;
	  }
  }
    return threeOfAKind;
}

int is_straight (hand_t hand) {

    int is_straight = 0;
    int i;
    sort_hand(hand);

/** Sorts the hand and checks whether the difference between successive terms is 1 **/
  if (hand[4].value != 14) {  
    if ( (hand[1].value - hand[0].value) == 1) { 
	  if ( (hand[2].value - hand[1].value) == 1) {
		if ( (hand[3].value - hand[2].value) == 1) {
			      if ( (hand[4].value - hand[3].value) == 1) {
				is_straight = 1; 
			      }
		}
	  }
    }
 }
    else {
     if ( (hand[1].value - hand[0].value) == 1) { 
	  if ( (hand[2].value - hand[1].value) == 1) {
		if ( (hand[3].value - hand[2].value) == 1) {
			is_straight = 1;
		}
	  }
     }
  }
    return is_straight;
}

int is_fullhouse (hand_t hand) {
    sort_hand(hand);
    int is_fullhouse = 0;
    if( (hand[2].value == hand[3].value) && (hand[3].value == hand[4].value) ) {
	    if (hand[0].value == hand[1].value) {
		    is_fullhouse = 1;
	    }
    }
    else if ( (hand[0].value == hand[1].value) && (hand[1].value == hand[2].value)) {
	   if (hand[3].value == hand[4].value) {
		  is_fullhouse = 1;
	   }
    }

    return is_fullhouse;
}

int is_flush (hand_t hand) {
   int is_flush = 0;
   if ((hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit) && (hand[3].suit == hand[4].suit)) {
	   is_flush = 1;
   }
	return is_flush;
}

int is_straightflush (hand_t hand) {
    sort_hand(hand);
    int is_straightflush = 0;
   if ( (hand[0].value == (hand[1].value - 1)) && (hand[1].value == (hand[2].value-1)) && (hand[2].value == (hand[3].value - 1)) && (hand[3].value == (hand[4].value -1))) {
if ( (hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit) && (hand[3].suit == hand[4].suit) ) {
	is_straightflush = 1;
}
}
    return is_straightflush;
}

int is_fourofakind (hand_t hand) {
    int is_fourofakind = 0;
    int i;
    for (i = 0; i < 2 ; i++ ) {
	    if ( (hand[i].value == hand[i + 1].value) && (hand[i + 1].value == hand[i + 2].value) && (hand[i + 2].value == hand[i + 3].value)) {
		    is_fourofakind = 1;
		    break;
	    }
    }
    return is_fourofakind;
}

int is_royalflush (hand_t hand) {
    sort_hand(hand);
    int is_royalflush = 0;
    if ( (hand[0].suit == hand[1].suit) && (hand[1].suit == hand[2].suit) && (hand[2].suit == hand[3].suit) && (hand[3].suit == hand[4].suit) ) {
	    if ( (hand[0].value ==  10) && (hand[1].value == 11) && (hand[2].value == 12) && (hand[3].value == 13) && (hand[4].value == 14) ) {
		    is_royalflush = 1;
	    }
    }
    return is_royalflush;
}

/* compares the hands based on rank -- if the ranks (and rank values) are
 * identical, compares the hands based on their highcards.
 * returns 0 if h1 > h2, 1 if h2 > h1.
 */
int compare_hands (hand_t h1, hand_t h2) {
	int h2Rank = 0;
	int h1Rank = 0;
/**Finds rank of first hand **/

	if (is_onepair(h1) == 1) {
		h1Rank = 0;
	}
	if (is_twopairs(h1) == 1) {
		h1Rank = 1;
	}
	if (is_threeofakind(h1) == 1) {
		h1Rank = 2;
	}
	if (is_straight(h1) == 1) {
		h1Rank = 3;
	}
	if (is_flush(h1) == 1) {
	        h1Rank = 4;
	}
	if (is_fullhouse(h1) == 1) {
		h1Rank = 5;
	}
	if (is_fourofakind(h1) == 1) {
		h1Rank = 6;
	}
	if (is_straightflush(h1) == 1) {
		h1Rank = 7;
	}
	if (is_royalflush(h1) == 1) {
		h1Rank = 8;
	}
	if (!is_onepair(h1) && !is_twopairs(h1) && !is_threeofakind(h1) && !is_straight(h1) && !is_flush(h1) && !is_fullhouse(h1) && !is_fourofakind(h1) && !is_straightflush(h1) && !is_royalflush(h1)) {
		h1Rank = -1;
	}

///////////////////////////////////////////////
     
   if (is_onepair(h2)) {
	    if (h1Rank == 0) { 
		   int i;
		   sort_hand(h1);
		   sort_hand(h2);
		   int card1 = h1[0].value, card2 = h2[0].value; 
		   for (i = 1; i<5; i++) {
			  if (card1 == h1[i].value)
				 break;
			  else 
				 card1 = h1[i].value;
		   }

		  for (i = 1; i < 5; i++) {
			 if (card2 == h2[i].value) 
				break;
			 else
				card2 = h2[i].value;
		  }
		 return (card2 > card1);
	    }
    	    else  h2Rank = 0;
     }

    if (is_twopairs(h2)) {
	
	if (h1Rank == 1) {
	   sort_hand(h1);
	   sort_hand(h2);
	   int i;
	   int k;
	   int h1Pair1, h1Pair2, h2Pair1, h2Pair2;
	   int highestH1, highestH2;

	   for (i = 0; i < 4; i++) {
		   if (h1[i].value == h1[i + 1].value) {
			   h1Pair1 = h1[i].value;
			   k = i + 2;
			   break;
		   }
	   }
	   for (i = k; i < 4; i++) {
		   if (h1[i].value == h1[i + 1].value) {
			   h1Pair2 = h1[i].value;
			   break;
		   }
	   }

	   for (i = 0; i < 4; i++) {
		   if (h2[i].value == h2[i + 1].value) {
			   h2Pair1 = h2[i].value;
			   k = i + 2;
			   break;
		   }
	   }
	   for (i = k; i < 4; i++) {
		   if (h2[i].value == h2[i + 1].value) {
			   h2Pair2 = h2[i].value;
			   break;
		   }
	   }

	   if (h1Pair1 > h1Pair2) {
		   highestH1 = h1Pair1;
	   }
	   else if (h1Pair2 > h1Pair1) {
		   highestH1 = h1Pair2;
	   }
	 
	   if (h2Pair1 > h2Pair2) {
		   highestH2 = h1Pair1;
	   }
	   else if (h2Pair2 > h2Pair1) {
		   highestH2 = h1Pair2;
	   }
	

	   if (highestH1 > highestH2) {
		   return 0;
	   }
	   else if (highestH2 > highestH1) {
		   return 1;
	   }
	}

     else h2Rank = 1; 
}

    if (is_threeofakind(h2)) {
	    if (h1Rank == 2) {
		    int card1 = h1[0].value, card2 = h2[0].value;
		    int i;
		    
		    for (i = 1; i < 5; i++) {
			    if (card2 == h1[i].value) 
				   break;
			    else 
				   card1 = h1[i].value;
		    }
		    for (i = 1; i < 5; i++) {
			    if (card2 == h2[i].value) 
				   break;
			    else 
				   card1 = h2[i].value;
		    }
		    return (card2 > card1);
	    }
	    else h2Rank = 2;
    }

    if (is_fullhouse(h2)) {
	    if (h1Rank == 5) {
		  int h2ThreeKindValue, h1ThreeKindValue;
		  
		  if ( (h1[0].value == h1[1].value) && (h1[1].value == h1[2].value) ) {
			   h1ThreeKindValue = h1[0].value;
		   }
		  else 
			   h1ThreeKindValue = h1[1].value;

		  if ( (h2[0].value == h2[1].value) && (h2[1].value == h2[2].value) ) {
			   h2ThreeKindValue = h2[0].value;
		   }
		  else 
			   h2ThreeKindValue = h2[1].value;

		  return (h2ThreeKindValue > h1ThreeKindValue);
	    }
	     else h2Rank = 5;
    }
    
    if (is_straight(h2) && !is_straightflush(h2)) {
	    if (h1Rank == 3) {	    
	  return compare_highcards(h1, h2);
	    }
	    else h2Rank = 3;
    }

    if (is_flush(h2)) {
	    if (h1Rank == 4) {
	   return compare_highcards(h1, h2);
	    }
	    else h2Rank = 4;
    }

    if (is_fourofakind(h2)) {
	 if (h1Rank == 6) {
	           int card1 = h1[0].value, card2 = h2[0].value;
		   int i;
		   for (i = 1; i<5; i++) {
			  if (card1 == h1[i].value)
				 break;
			  else 
				 card1 = h1[i].value;
		   }

		  for (i = 1; i < 5; i++) {
			 if (card2 == h2[i].value) 
				break;
			 else
				card2 = h2[i].value;
		  }
		 return (card2 > card1);
	    }
          else  h2Rank = 6;
   }
  
   if (is_straightflush(h2)) {
	   if (h1Rank == 7) {
    	   return compare_highcards(h1, h2);
	   }
	   else h2Rank = 7;
    }
   
   if (is_royalflush(h2)) {
	   h2Rank = 8;
    }

	if (!is_onepair(h2) && !is_twopairs(h2) && !is_threeofakind(h2) && !is_straight(h2) && !is_flush(h2) && !is_fullhouse(h2) && !is_fourofakind(h2) && !is_straightflush(h2) && !is_royalflush(h2)) {
		h2Rank = -1;
	}


/**Compares hands **/

	if (h1Rank > h2Rank) {
		return 0;
	}
        else if (h2Rank > h1Rank) {
		return 1;
	}
	else if (h2Rank == h1Rank) {
		return compare_highcards(h1, h2);
	}
}

/* compares the hands based solely on their highcard values (ignoring rank). if
 * the highcards are a draw, compare the next set of highcards, and so forth.
 */
int compare_highcards (hand_t h1, hand_t h2) {
  
    sort_hand(h1);
    sort_hand(h2);

if  ( h1[4].value > h2[4].value ) {
	return 0;
}
else if ( h1[4].value < h2[4].value ) {
    return 1;
}
else if  ( h1[3].value > h2[3].value ) {
	return 0;
}
else if ( h1[3].value < h2[3].value ) {
    return 1;
}
else if  ( h1[2].value > h2[2].value ) {
	return 0;
}
else if ( h1[2].value < h2[2].value ) {
    return 1;
}
else if  ( h1[1].value > h2[1].value ) {
	return 0;
}
else if ( h1[1].value < h2[1].value ) {
    return 1;
}
if  ( h1[0].value > h2[0].value ) {
	return 0;
}
else if ( h1[0].value < h2[0].value ) {
    return 1;
}


}
