#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>

using namespace eosio;

namespace transfer
{

    inline void transfer_native(name from, name to, asset quantity, const std::string &memo)
    {
        name token_contract = "eosio.token"_n; // The native token contract

        // Check if the token symbol is valid
        auto symbol = quantity.symbol;
        check(symbol.is_valid(), "Invalid token symbol");

        // Check if the quantity is positive
        check(quantity.amount > 0, "The transfer amount must be positive");

        // Perform the transfer using an inline action
        action(permission_level{from, "active"_n},
               token_contract, "transfer"_n,
               std::make_tuple(from, to, quantity, memo))
            .send();
    }

} // namespace