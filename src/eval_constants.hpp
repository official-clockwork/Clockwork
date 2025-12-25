#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(287, 340);
inline const PParam KNIGHT_MAT = S(917, 685);
inline const PParam BISHOP_MAT = S(868, 596);
inline const PParam ROOK_MAT   = S(1292, 1272);
inline const PParam QUEEN_MAT  = S(2710, 1855);
inline const PParam TEMPO_VAL  = S(59, 15);

inline const PParam BISHOP_PAIR_VAL   = S(80, 177);
inline const PParam ROOK_OPEN_VAL     = S(101, -27);
inline const PParam ROOK_SEMIOPEN_VAL = S(36, 15);

inline const PParam DOUBLED_PAWN_VAL = S(-36, -78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-66, -8);
inline const PParam OUTPOST_KNIGHT_VAL    = S(53, 32);
inline const PParam OUTPOST_BISHOP_VAL    = S(62, 22);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(48, 7);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(55, -22);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(34, 31);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(60, -50);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(21, 18), S(62, 30), S(76, 70), S(186, 145), S(538, 261), S(940, 1138)
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(66, 40), S(60, 31), S(67, 58), S(150, 121), S(689, -86)
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-77, -92), S(-77, -69), S(-51, 5), S(25, 75), S(89, 196), S(291, 304)
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(50, -44), S(33, -3), S(20, 28), S(27, 71), S(106, 142), S(152, 290)
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(13, -43), S(-1, 6), S(-3, -22), S(4, -44), S(-2, -89), S(-212, -130)
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-3, 114), S(-14, 85), S(-7, 32), S(2, 4), S(11, 9), S(38, 8), S(17, -5)
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-273, -17), S(-34, 19), S(-14, 45), S(29, 55), S(33, 82), S(50, 84), S(3, 102)
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-233, -223), S(-125, -63), S(-67, -10), S(-24, 24), S(23, 38), S(50, 76), S(88, 71), S(122, 73),
    S(168, 16)
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-245, -299), S(-172, -116), S(-101, -51), S(-68, 0), S(-39, 33), S(-23, 55), S(-6, 71), S(12, 77),
    S(30, 83), S(42, 80), S(66, 69), S(131, 22), S(158, 1), S(219, -32)
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-300, -251), S(-148, -97), S(-96, -33), S(-63, -24), S(-38, 0), S(-24, 22), S(-6, 34), S(12, 40),
    S(29, 53), S(46, 62), S(63, 65), S(74, 68), S(94, 71), S(106, 57), S(251, -70)
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-790, -500), S(-307, -661), S(-217, -505), S(-165, -301), S(-158, -95), S(-122, 11), S(-119, 121), S(-95, 133),
    S(-91, 189), S(-79, 213), S(-70, 238), S(-65, 253), S(-47, 244), S(-37, 255), S(-31, 249), S(-19, 244),
    S(-12, 234), S(-13, 240), S(12, 196), S(34, 157), S(51, 137), S(96, 69), S(108, 61), S(272, -118),
    S(308, -162), S(544, -315), S(373, -232), S(625, -368)
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(561, -76), S(168, -162), S(37, -56), S(0, -6), S(-50, 11), S(-112, 36), S(-159, 77), S(-207, 99),
    S(-242, 77)
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(206, 236), S(309, 188), S(383, 128)
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(378, 386), S(229, 246), S(144, 77)
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(414, 435), S(515, 416), S(539, 428), S(620, 462), S(695, 433)
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(1034, 998), S(716, 810), S(422, 616), S(204, 352), S(104, 38), S(26, -329)
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-110, 63), S(9, -18), S(-183, -134), S(34, 31), S(-279, -205)
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-27, 23), S(-21, 18), S(-25, 13), S(-15, 9), S(-22, -8)
};

inline const PParam PAWN_THREAT_KNIGHT = S(234, 62);
inline const PParam PAWN_THREAT_BISHOP = S(204, 104);
inline const PParam PAWN_THREAT_ROOK   = S(199, 56);
inline const PParam PAWN_THREAT_QUEEN  = S(175, -53);

inline const PParam KNIGHT_THREAT_BISHOP = S(106, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(246, 4);
inline const PParam KNIGHT_THREAT_QUEEN  = S(161, -70);

inline const PParam BISHOP_THREAT_KNIGHT = S(112, 34);
inline const PParam BISHOP_THREAT_ROOK   = S(245, 56);
inline const PParam BISHOP_THREAT_QUEEN  = S(192, 54);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(0, -12), S(-3, -6), S(-3, -16), S(-8, -27), S(-14, -32), S(-19, -40), S(-20, -47), S(-26, -42),
    S(-34, -48)
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(133, 152), S(99, 213), S(182, 175), S(246, 56), S(195, 49), S(186, 110), S(74, 134), S(153, 99),
    S(21, 52), S(113, 90), S(92, 36), S(98, -23), S(61, -44), S(11, 1), S(-24, 43), S(-60, 40),
    S(-23, 1), S(-15, 13), S(-2, -27), S(-7, -44), S(-18, -51), S(-54, -49), S(-93, -3), S(-111, 13),
    S(-29, -59), S(-17, -24), S(-19, -57), S(-35, -55), S(-57, -65), S(-75, -58), S(-125, -9), S(-143, -22),
    S(-33, -87), S(26, -78), S(-25, -31), S(-49, -33), S(-69, -43), S(-105, -47), S(-124, -33), S(-146, -40),
    S(-34, -76), S(90, -66), S(42, -27), S(-6, -14), S(-38, -30), S(-68, -40), S(-93, -15), S(-127, -28)
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-396, -157), S(-342, 56), S(-455, 232), S(-124, 68), S(-256, 96), S(-323, 92), S(-565, 82), S(-535, -22),
    S(5, -4), S(72, 9), S(165, -51), S(113, 11), S(120, 15), S(51, -3), S(10, 4), S(-11, -42),
    S(58, -28), S(84, 28), S(167, 25), S(121, 46), S(118, 37), S(46, 42), S(42, 9), S(-38, 8),
    S(107, 9), S(92, 37), S(109, 52), S(91, 77), S(104, 61), S(71, 54), S(58, 3), S(38, 5),
    S(100, -8), S(124, -4), S(109, 30), S(90, 39), S(78, 50), S(83, 39), S(60, 6), S(49, -56),
    S(28, -31), S(56, -40), S(49, -17), S(62, 26), S(70, 21), S(15, -3), S(21, -44), S(-16, -52),
    S(37, -20), S(59, -47), S(40, -39), S(39, -19), S(26, -26), S(-2, -46), S(11, -61), S(-47, -130),
    S(-15, -69), S(21, -24), S(44, -50), S(52, -43), S(43, -35), S(-5, -66), S(-18, -41), S(-65, -95)
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-179, 73), S(-197, 51), S(-441, 78), S(-318, 90), S(-275, 94), S(-436, 117), S(-175, 94), S(-132, 72),
    S(-4, -41), S(-17, 31), S(-2, 15), S(-35, 24), S(-55, 37), S(-4, 23), S(-24, 12), S(-61, 16),
    S(32, 13), S(71, 2), S(139, 15), S(69, 13), S(42, 18), S(24, 26), S(90, -6), S(-1, 10),
    S(37, -28), S(36, 11), S(73, 11), S(75, 35), S(86, 32), S(24, 30), S(21, 3), S(-18, 6),
    S(31, -51), S(51, -16), S(59, 1), S(61, 22), S(55, 37), S(22, 25), S(4, -12), S(-9, -53),
    S(54, -48), S(101, -38), S(111, -26), S(46, 16), S(32, 20), S(37, 13), S(62, -29), S(26, -48),
    S(45, -83), S(110, -69), S(80, -56), S(51, -25), S(38, -38), S(38, -48), S(21, -35), S(36, -96),
    S(45, -67), S(31, -18), S(45, -18), S(56, -52), S(64, -62), S(53, -17), S(48, -47), S(27, -48)
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(105, 5), S(173, 3), S(100, 33), S(97, 30), S(111, 17), S(62, 29), S(73, 30), S(80, 36),
    S(18, 59), S(101, 37), S(171, 18), S(94, 63), S(117, 47), S(68, 55), S(11, 70), S(5, 75),
    S(-4, 42), S(141, 3), S(165, 2), S(162, 0), S(123, 7), S(56, 43), S(78, 28), S(-39, 76),
    S(-28, 33), S(47, 28), S(74, 22), S(92, -12), S(70, 7), S(14, 51), S(-1, 49), S(-73, 57),
    S(-89, -16), S(-8, -10), S(-25, 8), S(-43, 10), S(-41, 2), S(-57, 40), S(-83, 35), S(-105, 26),
    S(-110, -38), S(-36, -62), S(-41, -34), S(-62, -32), S(-41, -53), S(-90, 0), S(-91, -19), S(-114, -21),
    S(-169, -29), S(-67, -90), S(-46, -73), S(-44, -69), S(-48, -66), S(-68, -49), S(-86, -73), S(-118, -56),
    S(-133, -31), S(-104, -23), S(-50, -58), S(-28, -73), S(-40, -59), S(-52, -48), S(-67, -57), S(-85, -40)
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(32, 30), S(72, -8), S(69, 0), S(-55, 132), S(17, 48), S(-28, 71), S(47, -15), S(-18, 15),
    S(20, 75), S(-52, 172), S(-63, 219), S(-165, 251), S(-136, 200), S(-119, 194), S(-70, 113), S(-33, 50),
    S(-23, 102), S(54, 99), S(-16, 178), S(-41, 187), S(-77, 169), S(-102, 172), S(-13, 63), S(-40, 36),
    S(42, 9), S(31, 78), S(-5, 116), S(-17, 176), S(-23, 156), S(-29, 98), S(13, 3), S(6, -24),
    S(9, 38), S(41, 3), S(15, 71), S(-20, 122), S(-24, 116), S(-17, 79), S(-1, -4), S(-1, -53),
    S(24, -114), S(50, -57), S(54, 4), S(2, 18), S(19, -17), S(26, -10), S(37, -80), S(8, -75),
    S(15, -212), S(53, -304), S(47, -181), S(61, -116), S(34, -92), S(51, -158), S(26, -95), S(7, -95),
    S(-40, -123), S(24, -374), S(29, -375), S(55, -291), S(56, -197), S(57, -245), S(44, -202), S(-11, -125)
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-281, -298), S(18, 20), S(-127, 72), S(-233, 98), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(110, -70), S(81, 88), S(126, 66), S(233, -3), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-76, 87), S(175, 70), S(243, 28), S(219, -13), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-284, 108), S(163, 23), S(173, 11), S(114, -6), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-256, 61), S(94, -10), S(149, -26), S(61, 7), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-190, 44), S(155, -48), S(113, -23), S(85, -7), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-30, -18), S(157, -63), S(110, -39), S(45, -6), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5),
    S(-230, 2), S(-49, -17), S(-127, 5), S(-141, 4), S(-19, -5), S(-19, -5), S(-19, -5), S(-19, -5)
};
// Epoch duration: 6.87929s
// clang-format on
}  // namespace Clockwork
