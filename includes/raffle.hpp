#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/crypto.hpp>

#include "../includes/transfer.hpp"

using namespace eosio;
using namespace transfer;

class [[eosio::contract]] raffle : public contract
{
public:
    using contract::contract;
    raffle(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds), raffles(receiver, receiver.value), winners(receiver, receiver.value), participants(receiver, receiver.value) {}

    [[eosio::action]] void create(name influencer, asset reward_amount);
    [[eosio::action]] void activate(uint64_t raffle_id, name by);
    [[eosio::action]] void participate(uint64_t raffle_id, name viewer);
    [[eosio::action]] void giveaway(uint64_t raffle_id);

    [[eosio::action]] void test(name something);

private:
    struct [[eosio::table]] raffle_entry
    {
        uint64_t id;
        eosio::name influencer;
        eosio::asset reward_amount;
        bool active;
        bool closed;
        eosio::time_point_sec created_at;

        uint64_t primary_key() const { return id; }
        uint64_t by_influencer() const { return influencer.value; }
    };

    typedef eosio::multi_index<"raffles"_n, raffle_entry,
                               indexed_by<"byinfluencer"_n, const_mem_fun<raffle_entry, uint64_t, &raffle_entry::by_influencer>>>
        raffles_table;
    raffles_table raffles;

    struct [[eosio::table]] winner_entry
    {
        uint64_t raffle_id;
        eosio::name winner;
        eosio::time_point_sec won_at;

        uint64_t primary_key() const { return raffle_id; }
        uint64_t by_winner() const { return winner.value; }
    };

    typedef eosio::multi_index<"winners"_n, winner_entry,
                               indexed_by<"bywinner"_n, const_mem_fun<winner_entry, uint64_t, &winner_entry::by_winner>>>
        winners_table;
    winners_table winners;

    struct [[eosio::table]] participant_entry
    {
        uint64_t id;
        uint64_t raffle_id;
        name viewer;
        eosio::time_point_sec participated_at;

        uint64_t primary_key() const { return id; }
        uint128_t by_raffle_part() const { return (static_cast<uint128_t>(raffle_id) << 64) | viewer.value; }
        uint64_t by_raffle_id() const { return raffle_id; }
        uint64_t by_viewer() const { return viewer.value; }
    };

    typedef eosio::multi_index<"participants"_n, participant_entry,
                               indexed_by<"byrafflepart"_n, const_mem_fun<participant_entry, uint128_t, &participant_entry::by_raffle_part>>,
                               indexed_by<"byraffleid"_n, const_mem_fun<participant_entry, uint64_t, &participant_entry::by_raffle_id>>,
                               indexed_by<"byviewer"_n, const_mem_fun<participant_entry, uint64_t, &participant_entry::by_viewer>>>
        participants_table;
    participants_table participants;

    void add_raffle(const name &influencer, const asset &reward_amount);
    void fund_raffle(const name &influencer, const asset &reward_amount);
    void add_participant(const name &participant, const uint64_t &raffle_id);
    void activate_raffle(const uint64_t &raffle_id, const name &by);
    void reward_winner(const name &winner, const raffle_entry &raffle);
    void close_raffle(const raffle_entry &raffle);
    const name pick_winner(const raffle_entry &raffle);
    void add_winner(const name &winner);
    uint64_t generate_random_number();
    uint64_t count_participants(const uint64_t raffle_id);
    name get_participant_by_index(const uint64_t raffle_id, const uint64_t index);
    bool is_viewer_participating(const name &viewer, const uint64_t raffle_id);

    // void only_raffle_contract();
    // void only_positive_reward_amount(const uint64_t &reward_amount);
    // void only_existing_raffle(const raffle_entry &raffle);
    // void only_active_raffle(const raffle_entry &raffle);
    // void only_inactive_raffle(const raffle_entry &raffle);
    // void only_not_raffle_influencer(const name &participant, const raffle_entry &raffle);
    // void only_raffle_influencer(const name &participant, const raffle_entry &raffle);
    // void only_not_already_participating(const name &viewer, const raffle_entry &raffle);
    // void only_not_closed_raffle(const raffle_entry &raffle);
};

// EOSIO_DISPATCH(raffle, (create))