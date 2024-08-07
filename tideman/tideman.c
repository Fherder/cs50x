#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
bool cycle(int i, int j);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int index, jndex;

    for (int i = 0; i < candidate_count; i++)
    {
        index = ranks[i];

        for (int j = i + 1; j < candidate_count; j++)
        {
            jndex = ranks[j];

            preferences[index][jndex]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;

                    pair_count++;
                }
            }
        }
    }

    /*for(int i=0;i<pair_count;i++){
        printf("[%i] winner: %i loser: %i\n", i, pairs[i].winner, pairs[i].loser);
    }*/

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair aux_pair;

    int i, j, most_voted, less_voted, winner, loser, winner_index, aux1, aux2, aux_index;

    bool swap = false;

    most_voted = 0;
    less_voted = 0;

    i = 0;
    winner_index = i;
    j = 1;

    while (i < pair_count && pair_count != 1)
    {
        aux1 = pairs[j].winner;
        aux2 = pairs[j].loser;

        less_voted = preferences[aux1][aux2];

        if (less_voted > most_voted)
        {
            aux_pair.winner = aux1;
            aux_pair.loser = aux2;

            winner_index = j;
            most_voted = less_voted;

            swap = true;
        }

        j++;

        if (j > pair_count - 1)
        {
            if (swap == false)
            {
                break;
            }

            pairs[winner_index].winner = pairs[i].winner;
            pairs[winner_index].loser = pairs[i].loser;

            pairs[i].winner = aux_pair.winner;
            pairs[i].loser = aux_pair.loser;

            swap = false;

            i++;

            winner = pairs[i].winner;
            loser = pairs[i].loser;

            most_voted = preferences[winner][loser];

            j = i + 1;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int index1, index2;

    index1 = pairs[0].winner;
    index2 = pairs[0].loser;

    locked[index1][index2] = true;

    for (int i = 1; i < pair_count; i++)
    {
        index1 = pairs[i].winner;
        index2 = pairs[i].loser;

        if (!cycle(index1, index2))
        {
            locked[index1][index2] = true;
        }
    }

    /*for(int i=0;i<candidate_count;i++){
        for(int j=0;j<candidate_count;j++){
            printf("%s ",  locked[i][j]? "true" : "false");
        }
        printf("\n");
    }*/

    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool winner[pair_count];

    for (int i = 0; i < pair_count; i++)
    {
        winner[i] = true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                winner[j] = false;
            }
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (winner[i] == true)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}

bool cycle(int i, int j)
{
    if (i == j)
    {
        return true;
    }

    for (int z = 0; z < candidate_count; z++)
    {
        if (locked[j][z] == true)
        {
            if (cycle(i, z))
            {
                return true;
            }
        }
    }

    return false;
}
