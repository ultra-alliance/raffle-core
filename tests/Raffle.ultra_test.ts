import makeTest from "@ultra-alliance/ultradev/dist/esm/functions/makeTest";
import { Services } from "../typegen";

module.exports = makeTest<unknown, Services>(
  {},
  ({ ultratest, getServices, getRequiredAccounts, assert }) => {
    const { common, keychain, endpoint } = ultratest;
    const accounts = getRequiredAccounts();
    const { bob, alice } = accounts;
    const { raffle } = getServices();

    const REWARD_AMOUNT = "100.00000000 UOS";

    return {
      "Logging involved accounts access keys:": async () => {
        console.log("\nACCOUNTS:\n");
        console.table(accounts);
      },
      "it should setup code permissions": async () => {
        await raffle
          .connect({
            name: raffle.name,
            privateKey: keychain.getPrivateKeyFromAccount(raffle.name) || "",
          })
          .addUosPerm(raffle.name);
        await raffle.connect(bob).addUosPerm();
        await raffle.connect(alice).addUosPerm();
        await common.sleep(1000);
      },
      "it should create a new raffle with liquidity": async () => {
        await raffle.create({
          influencer: alice.name,
          reward_amount: REWARD_AMOUNT,
        });
      },
      "it should have correct raffle data": async () => {
        const raffleData = await raffle.getRafflesTable({
          lowerBound: "0",
          upperBound: "-1",
        });
        console.log("raffleData", raffleData);
        assert(raffleData).toBe.notNull("Raffle data should be present");
        assert(raffleData?.rows[0].influencer).toBe.equal(
          alice.name,
          "Influencer should be Alice"
        );
        assert(raffleData?.rows[0].reward_amount).toBe.equal(
          REWARD_AMOUNT,
          "Reward amount should be 100"
        );
        assert(raffleData?.rows[0].active).toBe.equal(
          0,
          "Raffle should not be active"
        );
      },
      "it should become active upon influencer activation": async () => {
        await raffle.activate({
          raffle_id: 0,
          by: alice.name,
        });
        await common.sleep(1000);
        const raffleData = await raffle.getRafflesTable({
          lowerBound: "0",
          upperBound: "-1",
        });
        assert(raffleData?.rows[0].active).toBe.equal(
          1,
          "Raffle should be active"
        );
      },
      "it should allow viewer to participate": async () => {
        await raffle.connect(bob).participate({
          raffle_id: 0,
          viewer: bob.name,
        });
        await common.sleep(1000);
        const participantData = await raffle.getParticipantsTable({
          lowerBound: "0",
          upperBound: "-1",
        });
        assert(participantData).toBe.notNull(
          "Participant data should be present"
        );
        assert(participantData?.rows[0].viewer).toBe.equal(
          bob.name,
          "Viewer should be Bob"
        );
        const raffleBalanceBefore = await common.getBalance(raffle.name);
        console.log("raffleBalanceBefore", raffleBalanceBefore);
      },
      "it should close the raffle on giveaway and transfer reward":
        async () => {
          const raffleBalanceBefore = await common.getBalance(raffle.name);
          console.log("raffleBalanceBefore", raffleBalanceBefore);

          await raffle.giveaway({
            raffle_id: 0,
          });

          await common.sleep(1000);

          const raffleBalanceAfter = await common.getBalance(raffle.name);

          console.log("raffleBalanceAfter", raffleBalanceAfter);

          const raffleData = await raffle.getRafflesTable({
            lowerBound: "0",
            upperBound: "-1",
          });
          assert(raffleData?.rows[0].closed).toBe.equal(
            1,
            "Raffle should be closed"
          );
        },
    };
  }
);
