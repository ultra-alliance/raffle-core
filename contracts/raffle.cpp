#include "../includes/raffle.hpp";

void raffle::test(name something)
{
    add_raffle(get_self(),
               asset(1000000, symbol("UOS", 8)));
}

void raffle::create(name influencer, asset reward_amount)
{
    // require_auth(influencer);
    fund_raffle(influencer, reward_amount);
    add_raffle(influencer, reward_amount);
}

void raffle::activate(uint64_t raffle_id, name by)
{
    // require_auth(by);
    activate_raffle(raffle_id, by);
}

void raffle::participate(uint64_t raffle_id, name viewer)
{
    // require_auth(viewer);
    add_participant(viewer, raffle_id);
}

void raffle::giveaway(uint64_t raffle_id)
{

    auto raffle = raffles.find(raffle_id);
    // raffle::only_existing_raffle(*raffle);
    // raffle::only_active_raffle(*raffle);
    // raffle::only_not_closed_raffle(*raffle);

    name winner = pick_winner(*raffle);
    add_winner(winner);
    reward_winner(winner, *raffle);

    close_raffle(*raffle);
}

void raffle::add_raffle(
    const name &influencer,
    const asset &reward_amount)
{
    // raffle::only_raffle_contract();

    raffles.emplace(influencer, [&](auto &row)
                    {
        row.id = raffles.available_primary_key();
        row.influencer = influencer;
        row.reward_amount = reward_amount;
        row.active = false;
        row.closed = false;
        row.created_at = time_point_sec(0); });
}

void raffle::fund_raffle(
    const name &influencer,
    const asset &reward_amount)
{
    // raffle::only_raffle_contract();
    // raffle::only_positive_reward_amount(reward_amount);

    transfer::transfer_native(influencer, get_self(), reward_amount, "raffle reward");
}

void raffle::close_raffle(const raffle_entry &raffle)
{
    raffles.modify(raffle, get_self(), [&](auto &row)
                   { row.closed = true; });
}

void raffle::reward_winner(const name &winner, const raffle_entry &raffle)
{
    // raffle::only_raffle_contract();
    transfer::transfer_native(get_self(), winner, raffle.reward_amount, "raffle winner reward");
}

void raffle::add_participant(
    const name &participant,
    const uint64_t &raffle_id)
{
    //  raffle::only_raffle_contract();

    // auto raffle = raffles.find(raffle_id);

    // raffle::only_existing_raffle(*raffle);
    // raffle::only_active_raffle(*raffle);
    // raffle::only_not_closed_raffle(*raffle);
    //  raffle::only_not_raffle_influencer(participant, *raffle);
    // raffle::only_not_already_participating(participant, *raffle);

    participants.emplace(get_self(), [&](auto &row)
                         {
    row.id = participants.available_primary_key();
    row.raffle_id = raffle_id;
    row.viewer = participant;
    row.participated_at = current_time_point(); });
}

void raffle::activate_raffle(
    const uint64_t &raffle_id,
    const name &by)
{
    // raffle::only_raffle_contract();
    auto raffle = raffles.find(raffle_id);
    // raffle::only_existing_raffle(*raffle);
    // raffle::only_raffle_influencer(by, *raffle);
    // raffle::only_inactive_raffle(*raffle);

    raffles.modify(raffle, get_self(), [&](auto &row)
                   { row.active = true;
                  row.created_at = current_time_point(); });
}

const name raffle::pick_winner(const raffle_entry &raffle)
{
    // raffle::only_raffle_contract();
    // raffle::only_active_raffle(raffle);
    uint64_t num_participants = count_participants(raffle.id);
    uint64_t winner_index = generate_random_number() % num_participants;
    name winner = get_participant_by_index(raffle.id, winner_index);

    return winner;
}

void raffle::add_winner(const name &winner)
{
    winners.emplace(get_self(), [&](auto &row)
                    {
    row.raffle_id = winners.available_primary_key();
    row.winner = winner;
    row.won_at = current_time_point(); });
}

uint64_t raffle::generate_random_number()
{
    uint64_t seed_value = eosio::current_time_point().sec_since_epoch();
    eosio::checksum256 result = eosio::sha256((char *)&seed_value, sizeof(seed_value));
    return result.extract_as_byte_array()[0];
}

uint64_t raffle::count_participants(const uint64_t raffle_id)
{
    auto raffle_id_index = participants.get_index<"byraffleid"_n>();
    auto lower_bound_itr = raffle_id_index.lower_bound(raffle_id);
    auto upper_bound_itr = raffle_id_index.upper_bound(raffle_id);

    uint64_t num_participants = std::distance(lower_bound_itr, upper_bound_itr);
    return num_participants;
}

name raffle::get_participant_by_index(const uint64_t raffle_id, const uint64_t index)
{
    auto raffle_id_index = participants.get_index<"byraffleid"_n>();
    auto lower_bound_itr = raffle_id_index.lower_bound(raffle_id);
    auto upper_bound_itr = raffle_id_index.upper_bound(raffle_id);

    uint64_t num_participants = std::distance(lower_bound_itr, upper_bound_itr);
    eosio::check(index < num_participants, "Index is out of range.");

    std::advance(lower_bound_itr, index);
    return lower_bound_itr->viewer;
}

bool raffle::is_viewer_participating(const name &viewer, const uint64_t raffle_id)
{
    uint128_t raffle_viewer_key = (static_cast<uint128_t>(raffle_id) << 64) | viewer.value;
    auto by_raffle_viewer_index = participants.get_index<"byrafflepart"_n>();
    auto itr = by_raffle_viewer_index.find(raffle_viewer_key);

    return itr != by_raffle_viewer_index.end();
}

// void raffle::only_raffle_contract()
// {
//   check(get_first_receiver() == get_self(), "only raffle contract");
// }

// void raffle::only_positive_reward_amount(const uint64_t &reward_amount)
// {
//   check(reward_amount > 0, "reward amount must be positive");
// }

// void raffle::only_existing_raffle(
//     const raffle_entry &raffle)
// {
//   check(raffles.find(raffle.id) != raffles.end(), "raffle does not exist");
// }

// void raffle::only_active_raffle(
//     const raffle_entry &raffle)
// {
//   check(raffle.active, "raffle is not active");
// }

// void raffle::only_inactive_raffle(
//     const raffle_entry &raffle)
// {
//   check(!raffle.active, "raffle is active");
// }

// void raffle::only_not_raffle_influencer(
//     const name &influencer,
//     const raffle_entry &raffle)
// {
//   check(raffle.influencer != influencer, "influencer cannot participate in their own raffle");
// }

// void raffle::only_raffle_influencer(
//     const name &influencer,
//     const raffle_entry &raffle)
// {
//   check(raffle.influencer == influencer, "only raffle influencer");
// }

// void raffle::only_not_closed_raffle(
//     const raffle_entry &raffle)
// {
//   check(!raffle.closed, "raffle is closed");
// }

// void raffle::only_not_already_participating(
//     const name &viewer,
//     const raffle_entry &raffle)
// {
//   bool is_participating = raffle::is_viewer_participating(viewer, raffle.id);
//   check(!is_participating, "participant is already participating in this raffle");
// }