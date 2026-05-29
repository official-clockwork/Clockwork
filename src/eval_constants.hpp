#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(187, 494);
inline const PParam KNIGHT_MAT = S(801, 1536);
inline const PParam BISHOP_MAT = S(846, 1634);
inline const PParam ROOK_MAT   = S(1009, 2571);
inline const PParam QUEEN_MAT  = S(2391, 3894);
inline const PParam TEMPO_VAL  = S(63, 38);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -7);
inline const PParam BISHOP_PAIR_VAL   = S(63, 222);
inline const PParam ROOK_OPEN_VAL     = S(107, -12);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, -1);
inline const PParam MINOR_BEHIND_PAWN = S(15, 36);
inline const PParam RESTRICTED_SQUARES = S(-24, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-20, -79);
inline const PParam ISOLATED_PAWN_VAL = S(-14, -37);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -39);
inline const PParam OUTPOST_KNIGHT_VAL    = S(58, 38);
inline const PParam OUTPOST_BISHOP_VAL    = S(48, 38);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(36, 55);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(39, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(24, 54);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(58, -41);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(15, -3), S(30, 37), S(58, 66), S(127, 193), S(423, 279), S(579, 600),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(45, 35), S(39, 27), S(57, 57), S(108, 168), S(414, 82),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-90, -266), S(-88, -231), S(-66, -94), S(-24, 41), S(73, 232), S(208, 399),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(26, -45), S(25, -6), S(15, 36), S(14, 113), S(79, 238), S(152, 415),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -40), S(4, 7), S(-5, -24), S(-3, -59), S(-9, -161), S(-165, -320),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(13, 245), S(0, 188), S(-1, 112), S(9, 72), S(20, 74), S(59, 72), S(47, 55),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-130, -53), S(16, 5), S(2, 106), S(27, 154), S(38, 190), S(51, 194), S(34, 177),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-84, -360), S(-25, -131), S(5, -16), S(28, 25), S(56, 50), S(71, 83), S(89, 87), S(107, 93), S(126, 44),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-59, -270), S(-12, -106), S(37, -36), S(58, 12), S(80, 48), S(92, 73), S(98, 92), S(103, 108), S(107, 118), S(113, 121), S(122, 113), S(137, 92), S(142, 95), S(121, 62),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(107, -198), S(23, -12), S(43, 21), S(59, 36), S(70, 55), S(73, 69), S(78, 83), S(84, 88), S(89, 100), S(95, 106), S(101, 110), S(105, 114), S(108, 117), S(112, 106), S(141, 56),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-210, -129), S(-61, -21), S(-36, 101), S(-13, 221), S(2, 265), S(9, 316), S(15, 350), S(21, 366), S(25, 391), S(27, 410), S(32, 417), S(35, 427), S(38, 431), S(39, 437), S(39, 438), S(35, 442), S(31, 442), S(32, 432), S(31, 428), S(34, 418), S(28, 403), S(30, 379), S(-0, 405), S(-42, 385), S(-75, 390), S(-111, 410), S(-157, 429), S(-128, 345),
};

inline const PParam PAWN_THREAT_KNIGHT = S(199, 152);
inline const PParam PAWN_THREAT_BISHOP = S(192, 219);
inline const PParam PAWN_THREAT_ROOK   = S(204, 161);
inline const PParam PAWN_THREAT_QUEEN  = S(174, 29);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(7, 70), S(96, 91), S(112, 127), S(213, 137), S(179, 33), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(4, 67), S(67, 85), S(78, 95), S(-18, 30), S(204, -13), S(0, 0),
};
inline const PParam KING_THREAT  = S(-11, 178);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(5, -19), S(4, -15), S(-0, -21), S(-6, -29), S(-11, -37), S(-16, -44), S(-18, -57), S(-24, -60), S(-27, -79),
};

inline const PParam ROOK_LINEUP = S(15, 75);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(181, 267),    S(35, 375),     S(62, 352),     S(128, 259),    S(188, 159),    S(118, 220),    S(84, 259),     S(226, 201),    //
    S(34, 61),      S(39, 105),     S(23, 45),      S(38, -29),     S(32, -66),     S(4, -25),      S(-3, 32),      S(-25, 61),     //
    S(31, -14),     S(4, -3),       S(36, -50),     S(23, -72),     S(15, -91),     S(-7, -76),     S(-42, -39),    S(-42, -2),     //
    S(-9, -73),     S(-34, -41),    S(1, -65),      S(-8, -75),     S(-28, -86),    S(-38, -75),    S(-87, -52),    S(-78, -52),    //
    S(-12, -108),   S(21, -106),    S(7, -49),      S(-8, -55),     S(-34, -66),    S(-50, -74),    S(-78, -70),    S(-80, -74),    //
    S(10, -104),    S(82, -93),     S(70, -42),     S(25, -19),     S(-4, -44),     S(-23, -65),    S(-48, -59),    S(-60, -57),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-257, -520),  S(-255, 39),    S(-278, -81),   S(12, 20),      S(-84, 27),     S(-252, 59),    S(-381, 110),   S(-344, -395),  //
    S(-7, -28),     S(-7, 25),      S(98, 11),      S(76, 55),      S(89, 39),      S(52, 18),      S(-21, 19),     S(-31, -2),     //
    S(43, -38),     S(36, 29),      S(73, 55),      S(77, 67),      S(65, 68),      S(25, 67),      S(13, 26),      S(-36, 3),      //
    S(96, 11),      S(90, 39),      S(102, 68),     S(104, 107),    S(111, 108),    S(77, 77),      S(51, 38),      S(39, 24),      //
    S(74, 10),      S(105, 2),      S(102, 56),     S(93, 87),      S(80, 87),      S(79, 70),      S(65, 11),      S(24, 22),      //
    S(18, -42),     S(48, -17),     S(51, 30),      S(65, 62),      S(60, 60),      S(33, 25),      S(20, -8),      S(-23, -44),    //
    S(39, -30),     S(46, -21),     S(28, -12),     S(35, 17),      S(32, 12),      S(6, -39),      S(-26, -6),     S(-34, -112),   //
    S(-39, -93),    S(10, -32),     S(31, -46),     S(48, -36),     S(29, -13),     S(-7, -51),     S(-17, -32),    S(-58, -148),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-119, 12),    S(-169, 98),    S(-443, 192),   S(-276, 89),    S(-293, 135),   S(-296, 141),   S(-197, 109),   S(-119, 46),    //
    S(-28, -26),    S(-77, 94),     S(-38, 56),     S(-78, 74),     S(-67, 70),     S(-33, 44),     S(-6, 31),      S(-46, 10),     //
    S(34, 18),      S(11, 70),      S(30, 78),      S(28, 67),      S(22, 47),      S(20, 50),      S(11, 51),      S(17, 7),       //
    S(14, -13),     S(55, 17),      S(65, 41),      S(77, 61),      S(105, 45),     S(48, 29),      S(52, -5),      S(5, -13),      //
    S(50, -61),     S(50, -4),      S(81, 13),      S(100, 32),     S(82, 50),      S(71, 24),      S(24, 8),       S(21, -40),     //
    S(63, -44),     S(86, -29),     S(100, 8),      S(70, 38),      S(68, 19),      S(66, 21),      S(73, -10),     S(23, -28),     //
    S(35, -99),     S(116, -54),    S(75, -14),     S(52, 2),       S(35, 5),       S(55, -41),     S(53, -53),     S(47, -73),     //
    S(51, -90),     S(22, -19),     S(26, -5),      S(49, -31),     S(33, -23),     S(39, 11),      S(49, -23),     S(45, -65),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(163, 89),     S(204, 98),     S(150, 130),    S(138, 105),    S(199, 63),     S(156, 87),     S(175, 92),     S(145, 103),    //
    S(83, 139),     S(147, 132),    S(197, 102),    S(146, 100),    S(191, 81),     S(142, 109),    S(95, 135),     S(90, 137),     //
    S(60, 133),     S(180, 84),     S(208, 63),     S(184, 47),     S(182, 69),     S(128, 104),    S(130, 110),    S(73, 154),     //
    S(38, 99),      S(101, 109),    S(125, 82),     S(106, 84),     S(133, 73),     S(95, 113),     S(92, 121),     S(30, 146),     //
    S(9, 39),       S(79, 49),      S(66, 66),      S(41, 73),      S(50, 84),      S(39, 108),     S(17, 109),     S(-3, 107),     //
    S(21, -31),     S(92, -11),     S(80, 14),      S(61, 21),      S(71, 28),      S(40, 62),      S(48, 38),      S(1, 50),       //
    S(-66, -26),    S(69, -69),     S(71, -30),     S(59, 4),       S(62, 3),       S(47, 16),      S(37, -0),      S(8, 6),        //
    S(-3, -60),     S(9, -17),      S(81, -38),     S(91, -37),     S(90, -28),     S(68, -4),      S(67, -16),     S(49, -11),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(55, 163),     S(144, 112),    S(41, 268),     S(-11, 361),    S(26, 322),     S(60, 229),     S(60, 150),     S(-7, 203),     //
    S(50, 174),     S(61, 247),     S(24, 324),     S(-115, 455),   S(-60, 422),    S(-10, 322),    S(39, 175),     S(13, 166),     //
    S(0, 237),      S(32, 286),     S(0, 367),      S(-41, 411),    S(-22, 398),    S(29, 282),     S(61, 172),     S(52, 103),     //
    S(18, 142),     S(7, 254),      S(-23, 319),    S(-43, 400),    S(-25, 402),    S(9, 266),      S(50, 163),     S(25, 125),     //
    S(7, 138),      S(0, 176),      S(-14, 242),    S(-35, 314),    S(-22, 340),    S(-0, 261),     S(10, 174),     S(35, 80),      //
    S(7, 36),       S(29, 64),      S(23, 151),     S(-0, 175),     S(11, 177),     S(13, 210),     S(35, 130),     S(23, 87),      //
    S(-23, -112),   S(6, -95),      S(11, -13),     S(33, 25),      S(28, 68),      S(31, 42),      S(6, 71),       S(21, 47),      //
    S(-36, -95),    S(-20, -240),   S(9, -241),     S(25, -129),    S(32, -51),     S(33, -82),     S(31, -55),     S(4, -14),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(140, -595),   S(492, 2),      S(299, 163),    S(21, 126),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(356, -185),   S(379, 105),    S(200, 171),    S(23, 129),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(187, 45),     S(222, 159),    S(115, 187),    S(-34, 162),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(7, 13),       S(83, 122),     S(-30, 168),    S(-87, 180),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-114, -1),    S(-32, 80),     S(-101, 148),   S(-174, 191),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-49, -30),    S(12, 40),      S(-70, 118),    S(-121, 161),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(21, -83),     S(28, -3),      S(-33, 59),     S(-106, 109),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-22, -194),   S(2, -79),      S(-78, -18),    S(-72, -40),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-86, -436);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -6), S(10, 9), S(10, 3), S(5, 0), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 3), S(2, 3), S(3, 0), S(3, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, -18), S(-7, 6), S(-2, -9), S(9, -16), S(13, -28), S(6, -51), S(-4, -44), }},
  {{ S(2, 3), S(-22, 2), S(-17, 9), S(-9, 17), S(-7, 8), S(-9, -17), S(-19, -24), }},
  {{ S(-3, -1), S(-12, -9), S(-13, 18), S(-8, 18), S(-7, 10), S(-8, -17), S(-24, -40), }},
  {{ S(7, 8), S(-9, 16), S(-6, 37), S(-2, 43), S(-2, 38), S(5, 15), S(8, -15), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(9, 26), S(-5, 4), S(-8, 11), S(-5, 28), S(2, 48),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(5, 12), S(-44, -144), S(-10, -45), S(-0, 5), S(-2, 4), S(-5, 11), S(-5, 9), }},
  {{ S(9, -0), S(-25, -165), S(-6, -32), S(-3, 2), S(0, -2), S(-7, 5), S(2, 3), }},
  {{ S(-0, 10), S(-10, -88), S(6, -17), S(0, 9), S(-3, 11), S(-9, 19), S(-5, 20), }},
  {{ S(2, 1), S(5, -61), S(-2, 30), S(-3, 26), S(-6, 10), S(-11, 12), S(-10, 20), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1405, 875, -18, -6
);

inline VParam WINNABLE_PAWNS = V(-21);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(83);
inline VParam WINNABLE_PAWN_ENDGAME = V(107);
inline VParam WINNABLE_BIAS = V(-366);

// Epoch duration: 7.2013s
// clang-format on
}  // namespace Clockwork
