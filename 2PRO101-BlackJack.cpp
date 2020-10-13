#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <conio.h>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

constexpr int deeeeeeeeeeeelay = 4000;



struct CardType
{
    enum Type { Hearts, Diamonds, Clubs, Spades, size };
    enum Number { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, J, Q, K, A, Size };

    Type type;
    Number number;
};

struct Card
{
    int value;
    CardType cardFace;
    std::wstring display;
};

std::vector<Card> MainDeck;
std::vector<Card> DrawnCards;


Card GetRandomCard();
Card GetRandomCard()
{
    int randomint = rand() % MainDeck.size();
    DrawnCards.push_back(MainDeck.at(randomint));
    MainDeck.erase(MainDeck.begin() + randomint);
    return DrawnCards.at(DrawnCards.size() - 1);
}

class Hand 
{
    int handTotal = 0;
public:
    std::vector<Card> cards;
    void AddCard()
    {
        
        //Add card
        cards.emplace_back(GetRandomCard());
        //Add the cards value to the total
        Card& _card = cards.at(cards.size() - 1);
        handTotal += _card.value;

        // Checking the card if it matches Ace, if it does and the handTotal does not exceed 21. 
        if (_card.cardFace.number == CardType::A && handTotal > 21 )
        {
            handTotal -= 10;
        }
    }
    int CalcHandTotal()
    {
        return handTotal;
    }

};

// I am using these to easily display the right cards with the right Enum type in CardType.  
std::wstring TypeStrings[]{ L"Hearts", L"Diamonds", L"Clubs", L"Spades" };
// aswell as TypeStrings i do the same for NumberStrings. so that i can easily display it
std::wstring NumberStrings[]{ L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K", L"A" };

//This is storing the Main Deck of cards and the drawn cards globaly.

//This function grabs a random card from the deck, puts it in Used Cards and removes it from the main deck and returns the drawn card.


//this function takes the cards from the DrawnCards Vector and places them back on the bottom of the Main Deck. 
void ReturnCardsToDeck();
void ReturnCardsToDeck()
{
    for (int i = 0; i < DrawnCards.size(); i++)
    {
        MainDeck.push_back(DrawnCards.at(i));
    }
    DrawnCards.erase(DrawnCards.begin(), DrawnCards.end());
}
//This function takes care of Drawing the UI;
void DrawGFX(Hand&,Hand&, bool bShow = true);
void DrawGFX(Hand &Player, Hand &Dealer, bool bShow)
{
    system("CLS");
    for (int i = 0; i < Dealer.cards.size(); i++)
    {
        if (i == 0 && !bShow)
        {
            std::wstring n = L"----#--#----";
            std::wcout << n << " | ";

        }
        else
        { 
            std::wstring n = Dealer.cards.at(i).display.c_str();
            std::wcout << n << " | ";
            
        }
        
        
    }
    if (!bShow)
    {
        std::wcout << "\nDealer has : " << Dealer.CalcHandTotal() - Dealer.cards[0].value;
    }
    else
    {
        std::wcout << "\nDealer has : " << Dealer.CalcHandTotal();
    }
        
    std::wcout <<("\n\n\n");
    for (int i = 0; i < Player.cards.size(); i++)
    {
        std::wstring n = Player.cards.at(i).display;
        std::wcout << n << L" | ";
    }
    std::wcout << std::endl << "You have : " << Player.CalcHandTotal() << std::endl;

    std::wcout <<("\n 1. Hit \n 2. Stand\n");
}

int main()
{
    //This is for Unicode suppport. Thanks to Ali for the advice and help with understanding UNICODE
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    //Takes the current time as the random gen seed.
    srand(time(NULL));

    //Creating the deck of cards.
    //and filling it with the correct value aswell as 
    Card _Card;
    for (int i = 0; i < CardType::Type::size; i++)
    {
        for (int j = 0; j < CardType::Number::Size; j++)
        {
            //here im are filling out the details of the card and placing it in the MainDeck.
            //the deck gets made in order of color first, Hearts -> Diamonds -> Clubs -> Spades. And in each of those its added from 1 -> A.
            _Card.cardFace.type = (CardType::Type)i;
            _Card.cardFace.number = (CardType::Number)j;
            _Card.display = NumberStrings[j] + L" of " + TypeStrings[i];
            
            //Changes the value for a face card to 10 and Ace to 11. 
            switch (_Card.cardFace.number)
            {
            case CardType::J : 
                _Card.value = 10;
                break;
            case CardType::Q :
                _Card.value = 10;
                break;
            case CardType::K :
                _Card.value = 10;
                break;
            case CardType::A :
                _Card.value = 11;
                break;
            default:
                _Card.value = j + 2;
                break;
            }
            MainDeck.emplace_back(_Card);
        }
    }

    //initializing player and Dealer money. 
    int DealerMoney = 100;
    int PlayerMoney = 100;
    int CurrentBet = 0;
    //Game Loop
    do
    {
        //Clearing the screen and initializing both the player and the dealers hand. 
        
        Hand Dealer;
        Hand Player;

        ReturnCardsToDeck();

        //Resets the current bet.
        CurrentBet = 0;

        PlayerMoney -= 10;
        CurrentBet += 10;
        //This GOTO function is here incase the bid value the player entered does not work. 
        //Either if its Larger than the player can afford or if it is an invalid value    
    BID:
        system("CLS");
        int bet;
        std::wcout << L"Your Money : " << PlayerMoney << std::endl;
        std::wcout << L"Dealers Money : " << DealerMoney << std::endl << std::endl;
        std::wcout << L"Place Bed : ";
        std::cin >> bet;
        //this check is to make sure the player doesn't attempt an invalid bet. 
        if (bet > PlayerMoney || bet < 0 || !std::cin || bet > DealerMoney)
        {
            std::cin.clear();
            goto BID;
        }
        CurrentBet += bet;
        PlayerMoney -= bet;

        bool bRound = true;

        //The game is now underway. The player has made their bet and have also added the minimum buy-in which is hardcoded to 10.
        //The player and the dealer gets two cards. i then check if the player has a blackjack in which case the player wins.
        Player.AddCard();
        Player.AddCard();

        Dealer.AddCard();
        Dealer.AddCard();

        DrawGFX(Player, Dealer, false);
        if (Player.CalcHandTotal() == 21)
        {
            bRound = false;
            std::wcout << (L"\n---- | Player BlackJack | ----");
            PlayerMoney += (CurrentBet * 2) * 1.5;
            Sleep(deeeeeeeeeeeelay);

            continue;
        }

        //Round Loop
        do
        {
            // Gets the input from the player, and then goes into the Switch Statement. This then takes care of all the logic 
            switch (_getch())
            {
            case '1' :
                Player.AddCard();
                DrawGFX(Player, Dealer, false);
                if (Player.CalcHandTotal() == 21)
                {
                    //PLAYER WINS
                    bRound = false;
                    std::wcout << (L"\n---- | Player BlackJack | ----");
                    PlayerMoney += (CurrentBet * 2.5);
                    DealerMoney -= CurrentBet * 1.5;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                if (Player.CalcHandTotal() > 21)
                {
                    bRound = false;
                    
                    std::wcout << (L"\n---- | Player Bust :( | ----");
                    DealerMoney += CurrentBet;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                break;
            case '2' :
                for (;;)
                {
                    if (Dealer.CalcHandTotal() < 17)
                    {
                        Dealer.AddCard();
                    }
                    else
                    {
                        break;
                    }
                }
                DrawGFX(Player, Dealer);
                if (Dealer.CalcHandTotal() > 21)
                {
                    bRound = false;

                    std::wcout <<("\n---- | Dealer Bust :) | ----");
                    DealerMoney -= CurrentBet;
                    PlayerMoney += CurrentBet * 2;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                if(Dealer.CalcHandTotal() > Player.CalcHandTotal())
                { 
                    bRound = false;
                    std::wcout <<("\n----| House Wins |----");
                    DealerMoney += CurrentBet;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                else if (Dealer.CalcHandTotal() < Player.CalcHandTotal())
                {
                    bRound = false;
                    std::wcout <<("\n----| Player Wins |----");
                    PlayerMoney += CurrentBet * 2;
                    DealerMoney -= CurrentBet;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                else
                {
                    bRound = false;
                    std::wcout <<("\n----| DRAW |----");
                    PlayerMoney += CurrentBet;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                break;
            default:
                break;
            }
        } while (bRound);
    } while (PlayerMoney > 0 && DealerMoney > 0);
}