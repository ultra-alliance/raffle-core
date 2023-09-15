"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const BaseService_1 = __importDefault(require("@ultra-alliance/ultradev/dist/esm/services/BaseService"));
class RaffleService extends BaseService_1.default {
    constructor({ rpcEndpoint, signer, name, }) {
        super({ rpcEndpoint, signer, name: name || "rfflecntract" });
        this.name = name || "rfflecntract";
    }
    async activate(data) {
        return this.sendTransaction({ action: "activate", data });
    }
    async create(data) {
        return this.sendTransaction({ action: "create", data });
    }
    async giveaway(data) {
        return this.sendTransaction({ action: "giveaway", data });
    }
    async participate(data) {
        return this.sendTransaction({ action: "participate", data });
    }
    async test(data) {
        return this.sendTransaction({ action: "test", data });
    }
    activateRaw(data) {
        return this.populateTransaction({ action: "activate", data });
    }
    createRaw(data) {
        return this.populateTransaction({ action: "create", data });
    }
    giveawayRaw(data) {
        return this.populateTransaction({ action: "giveaway", data });
    }
    participateRaw(data) {
        return this.populateTransaction({ action: "participate", data });
    }
    testRaw(data) {
        return this.populateTransaction({ action: "test", data });
    }
    async getParticipantsTable({ index, keyType, lowerBound, upperBound, limit, }) {
        return this.queryTable({
            table: "participants",
            index,
            lowerBound,
            upperBound,
            keyType,
            limit,
        });
    }
    async getRafflesTable({ index, keyType, lowerBound, upperBound, limit, }) {
        return this.queryTable({
            table: "raffles",
            index,
            lowerBound,
            upperBound,
            keyType,
            limit,
        });
    }
    async getWinnersTable({ index, keyType, lowerBound, upperBound, limit, }) {
        return this.queryTable({
            table: "winners",
            index,
            lowerBound,
            upperBound,
            keyType,
            limit,
        });
    }
}
exports.default = RaffleService;
