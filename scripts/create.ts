import { RaffleService, DataTypes, ContractNames } from "../typegen";
import config from "../ultradev.config";
const { signer: alice } = config.network;

(async () => {
  const raffle = new RaffleService({
    rpcEndpoint: config.network.rpcEndpoint,
    signer: alice,
    name: ContractNames.ACCOUNT,
  });

  const RAFFLE_CREATION_PARAMS: DataTypes.CreateStruct = {
    influencer: alice.name,
    reward_amount: "100.00000000 UOS",
  };

  await raffle.create(RAFFLE_CREATION_PARAMS);

  const raffleCount = (await raffle.getRafflesTable({})).rows.length;

  console.log(`CREATED RAFFLE #: ${raffleCount}`);

  // const raffleCount = (await raffle.getRafflesTable({})).rows.length;

  // console.log("CREATED RAFFLE #: ", raffleCount + 1);
})();
