import BaseService from "@ultra-alliance/ultradev/dist/esm/services/BaseService";

import { Signer } from "@ultra-alliance/ultradev";

type Activate = {
  raffle_id: number;
  by: string;
};

type Create = {
  influencer: string;
  reward_amount: string;
};

type Giveaway = {
  raffle_id: number;
};

type ParticipantEntry = {
  id: number;
  raffle_id: number;
  viewer: string;
  participated_at: unknown;
};

type Participate = {
  raffle_id: number;
  viewer: string;
};

type RaffleEntry = {
  id: number;
  influencer: string;
  reward_amount: string;
  active: unknown;
  closed: unknown;
  created_at: unknown;
};

type Test = {
  something: string;
};

type WinnerEntry = {
  raffle_id: number;
  winner: string;
  won_at: unknown;
};

type tGetTable = {
  index?: string;
  keyType?: string;
  lowerBound?: string;
  upperBound?: string;
  limit?: number;
};

class RaffleService extends BaseService {
  name: string;

  constructor({
    rpcEndpoint,
    signer,
    name,
  }: {
    rpcEndpoint: string;
    signer: Signer;
    name?: string;
  }) {
    super({ rpcEndpoint, signer, name: name || "rfflecntract" });
    this.name = name || "rfflecntract";
  }

  async activate(data: Activate) {
    return this.sendTransaction({ action: "activate", data });
  }

  async create(data: Create) {
    return this.sendTransaction({ action: "create", data });
  }

  async giveaway(data: Giveaway) {
    return this.sendTransaction({ action: "giveaway", data });
  }

  async participate(data: Participate) {
    return this.sendTransaction({ action: "participate", data });
  }

  async test(data: Test) {
    return this.sendTransaction({ action: "test", data });
  }

  activateRaw(data: Activate) {
    return this.populateTransaction({ action: "activate", data });
  }

  createRaw(data: Create) {
    return this.populateTransaction({ action: "create", data });
  }

  giveawayRaw(data: Giveaway) {
    return this.populateTransaction({ action: "giveaway", data });
  }

  participateRaw(data: Participate) {
    return this.populateTransaction({ action: "participate", data });
  }

  testRaw(data: Test) {
    return this.populateTransaction({ action: "test", data });
  }

  async getParticipantsTable({
    index,
    keyType,
    lowerBound,
    upperBound,
    limit,
  }: tGetTable) {
    return this.queryTable<ParticipantEntry>({
      table: "participants",
      index,
      lowerBound,
      upperBound,
      keyType,
      limit,
    });
  }

  async getRafflesTable({
    index,
    keyType,
    lowerBound,
    upperBound,
    limit,
  }: tGetTable) {
    return this.queryTable<RaffleEntry>({
      table: "raffles",
      index,
      lowerBound,
      upperBound,
      keyType,
      limit,
    });
  }

  async getWinnersTable({
    index,
    keyType,
    lowerBound,
    upperBound,
    limit,
  }: tGetTable) {
    return this.queryTable<WinnerEntry>({
      table: "winners",
      index,
      lowerBound,
      upperBound,
      keyType,
      limit,
    });
  }
}

export default RaffleService;
