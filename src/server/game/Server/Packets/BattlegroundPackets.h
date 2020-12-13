/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BattlegroundPackets_h__
#define BattlegroundPackets_h__

#include "ArenaHelper.h"
#include "Packet.h"
#include "LFGPacketsCommon.h"
#include "ObjectGuid.h"
#include "Optional.h"
#include "PacketUtilities.h"
#include "Position.h"
#include <array>

namespace WorldPackets
{
    namespace Battleground
    {
        class PVPSeason final : public ServerPacket
        {
        public:
            PVPSeason() : ServerPacket(SMSG_PVP_SEASON, 4 + 4 + 4 + 4 + 4 + 1) { }

            WorldPacket const* Write() override;

            int32 MythicPlusSeasonID = 0;
            int32 PreviousSeason = 0;
            int32 CurrentSeason = 0;
            int32 PvpSeasonID = 0;
            int32 ConquestWeeklyProgressCurrencyID = 0;
            bool WeeklyRewardChestsEnabled = false;
        };

        class AreaSpiritHealerQuery final : public ClientPacket
        {
        public:
            AreaSpiritHealerQuery(WorldPacket&& packet) : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUERY, std::move(packet)) { }

            void Read() override;

            ObjectGuid HealerGuid;
        };

        class AreaSpiritHealerQueue final : public ClientPacket
        {
        public:
            AreaSpiritHealerQueue(WorldPacket&& packet) : ClientPacket(CMSG_AREA_SPIRIT_HEALER_QUEUE, std::move(packet)) { }

            void Read() override;

            ObjectGuid HealerGuid;
        };

        class AreaSpiritHealerTime final : public ServerPacket
        {
        public:
            AreaSpiritHealerTime() : ServerPacket(SMSG_AREA_SPIRIT_HEALER_TIME, 14 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid HealerGuid;
            int32 TimeLeft = 0;
        };

        class HearthAndResurrect final : public ClientPacket
        {
        public:
            HearthAndResurrect(WorldPacket&& packet) : ClientPacket(CMSG_HEARTH_AND_RESURRECT, std::move(packet)) { }

            void Read() override { }
        };

        class PVPLogDataRequest final : public ClientPacket
        {
        public:
            PVPLogDataRequest(WorldPacket&& packet) : ClientPacket(CMSG_PVP_LOG_DATA, std::move(packet)) { }

            void Read() override { }
        };

        struct PVPLogData
        {
            struct RatingData
            {
                int32 Prematch[2] = { };
                int32 Postmatch[2] = { };
                int32 PrematchMMR[2] = { };
            };

            struct HonorData
            {
                uint32 HonorKills = 0;
                uint32 Deaths = 0;
                uint32 ContributionPoints = 0;
            };

            struct PVPMatchPlayerPVPStat
            {
                PVPMatchPlayerPVPStat() : PvpStatID(0), PvpStatValue(0) { }
                PVPMatchPlayerPVPStat(int32 pvpStatID, int32 pvpStatValue) : PvpStatID(pvpStatID), PvpStatValue(pvpStatValue) { }

                int32 PvpStatID;
                int32 PvpStatValue;
            };

            struct PVPMatchPlayerStatistics
            {
                ObjectGuid PlayerGUID;
                uint32 Kills = 0;
                uint8 Faction = 0;
                bool IsInWorld = false;
                Optional<HonorData> Honor;
                uint32 DamageDone = 0;
                uint32 HealingDone = 0;
                Optional<uint32> PreMatchRating;
                Optional<int32> RatingChange;
                Optional<uint32> PreMatchMMR;
                Optional<int32> MmrChange;
                std::vector<PVPMatchPlayerPVPStat> Stats;
                int32 PrimaryTalentTree = 0;
                int32 Sex = 0;
                int32 Race = 0;
                int32 Class = 0;
                int32 CreatureID = 0;
                int32 HonorLevel = 0;
            };

            std::vector<PVPMatchPlayerStatistics> Statistics;
            Optional<RatingData> Ratings;
            std::array<int8, 2> PlayerCount = { };
        };

        class PVPLogDataMessage final : public ServerPacket
        {
        public:
            PVPLogDataMessage() : ServerPacket(SMSG_PVP_LOG_DATA, 0) { }

            WorldPacket const* Write() override;

            PVPLogData Data;
        };

        struct BattlefieldStatusHeader
        {
            WorldPackets::LFG::RideTicket Ticket;
            std::vector<uint64> QueueID;
            uint8 RangeMin = 0;
            uint8 RangeMax = 0;
            uint8 TeamSize = 0;
            uint32 InstanceID = 0;
            bool RegisteredMatch = false;
            bool TournamentRules = false;
        };

        class BattlefieldStatusNone final : public ServerPacket
        {
        public:
            BattlefieldStatusNone() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NONE, 16 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            WorldPackets::LFG::RideTicket Ticket;
        };

        class BattlefieldStatusNeedConfirmation final : public ServerPacket
        {
        public:
            BattlefieldStatusNeedConfirmation() : ServerPacket(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 4 + 4 + sizeof(BattlefieldStatusHeader) + 1) { }

            WorldPacket const* Write() override;

            uint32 Timeout = 0;
            uint32 Mapid = 0;
            BattlefieldStatusHeader Hdr;
            uint8 Role = 0;
        };

        class BattlefieldStatusActive final : public ServerPacket
        {
        public:
            BattlefieldStatusActive() : ServerPacket(SMSG_BATTLEFIELD_STATUS_ACTIVE, sizeof(BattlefieldStatusHeader) + 4 + 1 + 1 + 4 + 4) { }

            WorldPacket const* Write() override;

            BattlefieldStatusHeader Hdr;
            uint32 ShutdownTimer = 0;
            uint8 ArenaFaction = 0;
            bool LeftEarly = false;
            uint32 StartTimer = 0;
            uint32 Mapid = 0;
        };

        class BattlefieldStatusQueued final : public ServerPacket
        {
        public:
            BattlefieldStatusQueued() : ServerPacket(SMSG_BATTLEFIELD_STATUS_QUEUED, 4 + sizeof(BattlefieldStatusHeader) + 1 + 1 + 1 + 4) { }

            WorldPacket const* Write() override;

            uint32 AverageWaitTime = 0;
            BattlefieldStatusHeader Hdr;
            bool AsGroup = false;
            bool SuspendedQueue = false;
            bool EligibleForMatchmaking = false;
            uint32 WaitTime = 0;
        };

        class BattlefieldStatusFailed final : public ServerPacket
        {
        public:
            BattlefieldStatusFailed() : ServerPacket(SMSG_BATTLEFIELD_STATUS_FAILED, 8 + 16 + 4 + 16 + 4 + 4 + 4) { }

            WorldPacket const* Write() override;

            uint64 QueueID = 0;
            ObjectGuid ClientID;
            int32 Reason = 0;
            WorldPackets::LFG::RideTicket Ticket;
        };

        class BattlemasterJoin final : public ClientPacket
        {
        public:
            BattlemasterJoin(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN, std::move(packet)) { }

            void Read() override;

            Array<uint64, 1> QueueIDs;
            uint8 Roles = 0;
            int32 BlacklistMap[2] = { };
        };

        class BattlemasterJoinArena final : public ClientPacket
        {
        public:
            BattlemasterJoinArena(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN_ARENA, std::move(packet)) { }

            void Read() override;

            uint8 TeamSizeIndex = 0;
            uint8 Roles = 0;
        };

        class BattlemasterJoinArenaSkirmish final : public ClientPacket
        {
        public:
            BattlemasterJoinArenaSkirmish(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEMASTER_JOIN_SKIRMISH, std::move(packet)) { }

            void Read() override;
        };

        class BattlefieldLeave final : public ClientPacket
        {
        public:
            BattlefieldLeave(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEFIELD_LEAVE, std::move(packet)) { }

            void Read() override { }
        };

        class BattlefieldPort final : public ClientPacket
        {
        public:
            BattlefieldPort(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEFIELD_PORT, std::move(packet)) { }

            void Read() override;

            WorldPackets::LFG::RideTicket Ticket;
            bool AcceptedInvite = false;
        };

        class BattlefieldListRequest final : public ClientPacket
        {
        public:
            BattlefieldListRequest(WorldPacket&& packet) : ClientPacket(CMSG_BATTLEFIELD_LIST, std::move(packet)) { }

            void Read() override;

            int32 ListID = 0;
        };

        class BattlefieldList final : public ServerPacket
        {
        public:
            BattlefieldList() : ServerPacket(SMSG_BATTLEFIELD_LIST, 1 + 1 + 16 + 1 + 1 + 1 + 4 + 1 + 4) { }

            WorldPacket const* Write() override;

            ObjectGuid BattlemasterGuid;
            int32 BattlemasterListID = 0;
            uint8 MinLevel = 0;
            uint8 MaxLevel = 0;
            std::vector<int32> Battlefields;    // Players cannot join a specific battleground instance anymore - this is always empty
            bool PvpAnywhere = false;
            bool HasRandomWinToday = false;
        };

        class GetPVPOptionsEnabled final : public ClientPacket
        {
        public:
            GetPVPOptionsEnabled(WorldPacket&& packet) : ClientPacket(CMSG_GET_PVP_OPTIONS_ENABLED, std::move(packet)) { }

            void Read() override { }
        };

        class PVPOptionsEnabled final : public ServerPacket
        {
        public:
            PVPOptionsEnabled() : ServerPacket(SMSG_PVP_OPTIONS_ENABLED, 1) { }

            WorldPacket const* Write() override;

            bool WargameArenas = false;
            bool RatedArenas = false;
            bool WargameBattlegrounds = false;
            bool ArenaSkirmish = false;
            bool PugBattlegrounds = false;
            bool RatedBattlegrounds = false;
        };

        class RequestBattlefieldStatus final : public ClientPacket
        {
        public:
            RequestBattlefieldStatus(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_BATTLEFIELD_STATUS, std::move(packet)) { }

            void Read() override { }
        };

        class ReportPvPPlayerAFK final : public ClientPacket
        {
        public:
            ReportPvPPlayerAFK(WorldPacket&& packet) : ClientPacket(CMSG_REPORT_PVP_PLAYER_AFK, std::move(packet)) { }

            void Read() override;

            ObjectGuid Offender;
        };

        class ReportPvPPlayerAFKResult final : public ServerPacket
        {
        public:
            ReportPvPPlayerAFKResult() : ServerPacket(SMSG_REPORT_PVP_PLAYER_AFK_RESULT, 16 + 1 + 1 + 1) { }

            WorldPacket const* Write() override;

            enum ResultCode : uint8
            {
                PVP_REPORT_AFK_SUCCESS = 0,
                PVP_REPORT_AFK_GENERIC_FAILURE = 1, // there are more error codes but they are impossible to receive without modifying the client
                PVP_REPORT_AFK_SYSTEM_ENABLED = 5,
                PVP_REPORT_AFK_SYSTEM_DISABLED = 6
            };

            ObjectGuid Offender;
            uint8 NumPlayersIHaveReported = 0;
            uint8 NumBlackMarksOnOffender = 0;
            uint8 Result = PVP_REPORT_AFK_GENERIC_FAILURE;
        };

        struct BattlegroundPlayerPosition
        {
            ObjectGuid Guid;
            TaggedPosition<Position::XY> Pos;
            int8 IconID = 0;
            int8 ArenaSlot = 0;
        };

        class BattlegroundPlayerPositions final : public ServerPacket
        {
        public:
            BattlegroundPlayerPositions() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_POSITIONS, 4) { }

            WorldPacket const* Write() override;

            std::vector<BattlegroundPlayerPosition> FlagCarriers;
        };

        class BattlegroundPlayerJoined final : public ServerPacket
        {
        public:
            BattlegroundPlayerJoined() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_JOINED, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class BattlegroundPlayerLeft final : public ServerPacket
        {
        public:
            BattlegroundPlayerLeft() : ServerPacket(SMSG_BATTLEGROUND_PLAYER_LEFT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class DestroyArenaUnit final : public ServerPacket
        {
        public:
            DestroyArenaUnit() : ServerPacket(SMSG_DESTROY_ARENA_UNIT, 16) { }

            WorldPacket const* Write() override;

            ObjectGuid Guid;
        };

        class RequestPVPRewards final : public ClientPacket
        {
        public:
            RequestPVPRewards(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_PVP_REWARDS, std::move(packet)) { }

            void Read() override { }
        };

        class RequestRatedPvpInfo final : public ClientPacket
        {
        public:
            RequestRatedPvpInfo(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_RATED_PVP_INFO, std::move(packet)) { }

            void Read() override { }
        };

        struct RatedInfo
        {
            uint32 ArenaPersonalRating;
            uint32 BestRatingOfWeek;
            uint32 BestRatingOfSeason;
            uint32 ArenaMatchMakerRating;
            uint32 WeekWins;
            uint32 WeekGames;
            uint32 PrevWeekWins;
            uint32 PrevWeekGames;
            uint32 SeasonWins;
            uint32 SeasonGames;
            uint32 ProjectedConquestCap;
            uint32 Ranking;
        };

        class RatedBattleFieldInfo final : public ServerPacket
        {
        public:
            RatedBattleFieldInfo() : ServerPacket(SMSG_RATED_BATTLEFIELD_INFO, MAX_PVP_SLOT * sizeof(RatedInfo)) { }

            WorldPacket const* Write() override;

            std::array<RatedInfo, MAX_PVP_SLOT> Infos;
        };

        class RequestConquestFormulaConstants final : public ClientPacket
        {
        public:
            RequestConquestFormulaConstants(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS, std::move(packet)) { }

            void Read() override { }
        };

        class ConquestFormulaConstants final : public ServerPacket
        {
        public:
            ConquestFormulaConstants() : ServerPacket(SMSG_CONQUEST_FORMULA_CONSTANTS, 20) { }

            WorldPacket const* Write() override;

            uint32 PvpMinCPPerWeek = 0;
            uint32 PvpMaxCPPerWeek = 0;
            float PvpCPBaseCoefficient = 0.0f;
            float PvpCPExpCoefficient = 0.0f;
            float PvpCPNumerator = 0.0f;
        };
	
        class SendPvpBrawlInfo final : public ServerPacket
        {
        public:
            SendPvpBrawlInfo() : ServerPacket(SMSG_REQUEST_PVP_BRAWL_INFO_RESPONSE) { }

            WorldPacket const* Write() override;

            uint32 BrawlType = 0;
            int32 TimeToEnd = 0;
            bool IsActive = false;
        };

        class RequestPvpBrawlInfo final : public ClientPacket
        {
        public:
            RequestPvpBrawlInfo(WorldPacket&& packet) : ClientPacket(CMSG_REQUEST_PVP_BRAWL_INFO, std::move(packet)) { }

            void Read() override { }
        };

        class PVPMatchInit final : public ServerPacket
        {
        public:
            PVPMatchInit() : ServerPacket(SMSG_PVP_MATCH_INIT, 4 + 1 + 4 + 4 + 1 + 4 + 1) { }

            WorldPacket const* Write() override;

            enum MatchState : uint8
            {
                InProgress = 1,
                Complete = 3,
                Inactive = 4
            };

            uint32 MapID = 0;
            MatchState State = Inactive;
            time_t StartTime = time_t(0);
            int32 Duration = 0;
            uint8 ArenaFaction = 0;
            uint32 BattlemasterListID = 0;
            bool Registered = false;
            bool AffectsRating = false;
        };

        class PVPMatchEnd final : public ServerPacket
        {
        public:
            PVPMatchEnd() : ServerPacket(SMSG_PVP_MATCH_END) { }

            WorldPacket const* Write() override;

            uint8 Winner = 0;
            int32 Duration = 0;
            Optional<PVPLogData> LogData;
        };
    }
}

#endif // BattlegroundPackets_h__
