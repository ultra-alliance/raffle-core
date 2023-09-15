import { UltraDevConfig } from "@ultra-alliance/ultradev/dist/esm";
import * as dotenv from "dotenv";
dotenv.config();

const ALICE_PRIVATE_KEY = String(process.env.ALICE_PRIVATE_KEY);
const ALICE_ACCOUNT_NAME = String(process.env.ALICE_ACCOUNT_NAME);

const config: UltraDevConfig = {
  network: {
    rpcEndpoint: "http://127.0.0.1:8888",
    signer: {
      name: ALICE_ACCOUNT_NAME,
      privateKey: ALICE_PRIVATE_KEY,
    },
  },
  testing: {
    requiresSystemContracts: true,
    importContracts: [
      {
        account: "rfflecntract",
        contract: "raffle",
      },
    ],
    requiredAccounts: ["alice", "bob", "carol", "dave", "eve", "frank"],
    requiredUnlimitedAccounts: ["zidane"],
  },
  directories: {
    sources: "contracts",
    artifacts: "artifacts",
    tests: "tests",
    scripts: "scripts",
    includes: "includes",
    ricardians: "resources",
  },
  typegen: {
    outdir: "./typegen",
    target: "eosjs",
  },
};

export default config;
