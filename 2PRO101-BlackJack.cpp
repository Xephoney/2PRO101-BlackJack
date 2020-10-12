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

class Hand 
{
    int handTotal = 0;
public:
    std::vector<Card> cards;
    void AddCard(Card _card)
    {
        //Add card
        cards.push_back(_card);
        //Add the cards value to the total
        handTotal += _card.value;

        // Checking the card if it matches Ace, if it does and the handTotal does not exceed 21. 
        if (_card.cardFace.number == CardType::A && handTotal > 21 )
        {
            handTotal -= 10;
        }
    }
    int CalculateHandSum()
    {
        return handTotal;
    }

};

std::wstring TypeString[]{ L"Hearts", L"Diamonds", L"Clubs", L"Spades" };
std::wstring NumberString[]{ L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K", L"A" };

std::vector<Card> AvailableCards;
std::vector<Card> UsedCards;

Card GetRandomCard();
Card GetRandomCard()
{
    int randomint = rand() % AvailableCards.size();
    UsedCards.push_back(AvailableCards.at(randomint));
    AvailableCards.erase(AvailableCards.begin() + randomint);
    return UsedCards.at(UsedCards.size()-1);
}
void ReShuffleDeck();
void ReShuffleDeck()
{
    for (int i = 0; i < UsedCards.size(); i++)
    {
        AvailableCards.push_back(UsedCards.at(i));
    }
    UsedCards.erase(UsedCards.begin(), UsedCards.end());
}
void DrawUI(Hand&,Hand&);
void DrawUI(Hand &Player, Hand &Dealer)
{
    system("CLS");
    for (int i = 0; i < Dealer.cards.size(); i++)
    {
        std::wstring n = Dealer.cards.at(i).display.c_str();
        /*std::wcout <<("|-------------------------------|\n"
            "|--------------%s---------------|\n"
            "|-------------------------------|\n"
            "|-------------------------------|\n", n);
            */
        std::wcout << n << " | ";
    }
    std::wcout <<("\n\n\n");
    for (int i = 0; i < Player.cards.size(); i++)
    {
        std::wstring n = Player.cards.at(i).display;
        /*std::wcout <<("|-------------------------------|\n"
            "|--------------%s---------------|\n"
            "|-------------------------------|\n"
            "|-------------------------------|\n", n);*/
        std::wcout << n << L" | ";
    }
    std::wcout << std::endl;

    std::wcout <<("\n 1. Hit \n 2. Stand\n");
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    srand(time(NULL));
    //Creating the deck of cards.
    //and filling it with the correct value aswell as 
    Card _Card;
    for (int i = 0; i < CardType::Type::size; i++)
    {
        for (int j = 0; j < CardType::Number::Size; j++)
        {
            _Card.cardFace.type = (CardType::Type)i;
            _Card.cardFace.number = (CardType::Number)j;
            _Card.display = NumberString[j] + L" of " + TypeString[i];
            
            //Changes the value for a face card to 10. 
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
            AvailableCards.push_back(_Card);
        }
    }

    int DealerMoney = 1000;
    int PlayerMoney = 1000;
    int CurrentBet = 0;
    //Game Loop
    do
    {
        system("CLS");
        Hand Dealer;
        Hand Player;

        ReShuffleDeck();
        CurrentBet = 0;

        PlayerMoney -= 10;
        CurrentBet += 10;
        BID:
        int bid;
        std::wcout << L"Your Money : " << PlayerMoney << std::endl;
        std::wcout << L"Place Bid : ";
        std::cin >> bid;
        if (bid > PlayerMoney || bid < 0 || !std::cin)
        {
            std::cin.clear();
            goto BID;
        }
        CurrentBet += bid;
        PlayerMoney -= bid;

        bool bRound = true;
        Player.AddCard(GetRandomCard());
        Player.AddCard(GetRandomCard());
        if (Player.CalculateHandSum() == 21)
        {
            bRound = false;
            std::wcout << (L"\n---- | Player BlackJack | ----");
            PlayerMoney += (CurrentBet * 2) * 1.5;
            Sleep(deeeeeeeeeeeelay);

            continue;
        }

        Dealer.AddCard(GetRandomCard());
        Dealer.AddCard(GetRandomCard());

        DrawUI(Player, Dealer);

        //Round Loop
        do
        {
            
            switch (_getch())
            {
            case '1' :
                Player.AddCard(GetRandomCard());
                DrawUI(Player, Dealer);
                if (Player.CalculateHandSum() == 21)
                {
                    //PLAYER WINS
                    bRound = false;
                    std::wcout << (L"\n---- | Player BlackJack | ----");
                    PlayerMoney += (CurrentBet * 2.5);
                    DealerMoney -= CurrentBet * 1.5;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                if (Player.CalculateHandSum() > 21)
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
                    if (Dealer.CalculateHandSum() < 17)
                    {
                        Dealer.AddCard(GetRandomCard());
                    }
                    else
                    {
                        break;
                    }
                }
                DrawUI(Player, Dealer);
                if (Dealer.CalculateHandSum() > 21)
                {
                    bRound = false;

                    std::wcout <<("\n---- | Dealer Bust :( | ----");
                    DealerMoney -= CurrentBet;
                    PlayerMoney += CurrentBet * 2;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                if(Dealer.CalculateHandSum() > Player.CalculateHandSum())
                { 
                    bRound = false;
                    std::wcout <<("\n----| House Wins |----");
                    DealerMoney += CurrentBet;
                    Sleep(deeeeeeeeeeeelay);
                    break;
                }
                else if (Dealer.CalculateHandSum() < Player.CalculateHandSum())
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
