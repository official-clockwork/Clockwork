#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(182, 438);
inline const PParam KNIGHT_MAT = S(836, 1255);
inline const PParam BISHOP_MAT = S(873, 1314);
inline const PParam ROOK_MAT   = S(1041, 2101);
inline const PParam QUEEN_MAT  = S(2548, 3250);
inline const PParam TEMPO_VAL  = S(61, 21);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, 14);
inline const PParam BISHOP_PAIR_VAL   = S(52, 221);
inline const PParam ROOK_OPEN_VAL     = S(115, -13);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 31);
inline const PParam MINOR_BEHIND_PAWN = S(14, 40);
inline const PParam RESTRICTED_SQUARES = S(-23, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-14, -77);
inline const PParam ISOLATED_PAWN_VAL = S(-17, -24);

inline const PParam POTENTIAL_CHECKER_VAL = S(-50, -24);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 56);
inline const PParam OUTPOST_BISHOP_VAL    = S(57, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 5);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(44, -15);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 52);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(60, -39);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(16, 3), S(29, 35), S(57, 60), S(126, 170), S(398, 241), S(464, 507),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(44, 42), S(43, 35), S(62, 59), S(116, 159), S(420, 85),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-97, -216), S(-98, -183), S(-75, -61), S(-27, 53), S(83, 208), S(244, 312),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(26, -40), S(23, -2), S(17, 33), S(10, 115), S(77, 231), S(140, 385),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -25), S(4, 19), S(-0, -17), S(-0, -49), S(-19, -133), S(-206, -253),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(21, 203), S(-3, 173), S(1, 97), S(7, 59), S(18, 57), S(57, 52), S(45, 40),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-209, -59), S(-9, 31), S(-2, 95), S(34, 125), S(51, 149), S(60, 157), S(37, 155),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-100, -298), S(-38, -106), S(-9, -5), S(15, 33), S(46, 54), S(61, 88), S(81, 88), S(101, 99), S(133, 26),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-70, -275), S(-22, -100), S(29, -25), S(52, 24), S(74, 61), S(86, 88), S(91, 109), S(99, 121), S(101, 137), S(113, 127), S(117, 126), S(154, 76), S(161, 65), S(186, 17),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(103, -141), S(23, 27), S(45, 57), S(61, 68), S(71, 83), S(76, 95), S(81, 107), S(88, 110), S(93, 122), S(100, 126), S(108, 129), S(110, 137), S(113, 136), S(120, 113), S(191, 7),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-162, -72), S(-33, 23), S(-8, 145), S(12, 255), S(27, 298), S(36, 343), S(41, 375), S(49, 384), S(53, 408), S(55, 425), S(60, 430), S(63, 440), S(69, 437), S(68, 446), S(70, 441), S(65, 445), S(63, 442), S(68, 428), S(71, 418), S(78, 400), S(76, 383), S(91, 341), S(71, 351), S(56, 298), S(29, 292), S(13, 267), S(-36, 285), S(-26, 222),
};

inline const PParam PAWN_THREAT_KNIGHT = S(204, 107);
inline const PParam PAWN_THREAT_BISHOP = S(192, 156);
inline const PParam PAWN_THREAT_ROOK   = S(200, 130);
inline const PParam PAWN_THREAT_QUEEN  = S(179, -14);

inline const PParam KNIGHT_THREAT_BISHOP = S(108, 114);
inline const PParam KNIGHT_THREAT_ROOK   = S(222, 87);
inline const PParam KNIGHT_THREAT_QUEEN  = S(154, -9);

inline const PParam BISHOP_THREAT_KNIGHT = S(98, 72);
inline const PParam BISHOP_THREAT_ROOK   = S(207, 142);
inline const PParam BISHOP_THREAT_QUEEN  = S(167, 108);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(8, -27), S(-0, -8), S(-3, -17), S(-6, -27), S(-11, -37), S(-14, -45), S(-16, -57), S(-21, -60), S(-35, -57),
};

inline const PParam ROOK_LINEUP = S(16, 63);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(214, 203),    S(84, 296),     S(136, 265),    S(153, 193),    S(195, 117),    S(109, 203),    S(87, 234),     S(211, 176),    //
    S(31, 40),      S(41, 83),      S(36, 13),      S(38, -48),     S(30, -80),     S(-5, -23),     S(-13, 32),     S(-37, 56),     //
    S(29, -22),     S(9, -16),      S(33, -59),     S(21, -86),     S(17, -101),    S(-6, -71),     S(-47, -26),    S(-48, 4),      //
    S(-7, -77),     S(-26, -53),    S(5, -75),      S(-7, -83),     S(-28, -86),    S(-34, -68),    S(-91, -34),    S(-80, -39),    //
    S(-6, -110),    S(30, -114),    S(16, -57),     S(-1, -61),     S(-29, -70),    S(-45, -66),    S(-77, -52),    S(-78, -57),    //
    S(14, -108),    S(92, -112),    S(71, -63),     S(31, -28),     S(3, -47),      S(-19, -59),    S(-48, -44),    S(-61, -42),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-302, -389),  S(-283, 46),    S(-236, -182),  S(-29, 44),     S(-99, 41),     S(-233, 34),    S(-397, 103),   S(-376, -271),  //
    S(-41, -14),    S(-24, 30),     S(88, -7),      S(86, 28),      S(87, 27),      S(23, 32),      S(-41, 36),     S(-60, 6),      //
    S(27, -21),     S(36, 13),      S(75, 61),      S(92, 59),      S(64, 82),      S(18, 81),      S(7, 19),       S(-37, 9),      //
    S(89, 12),      S(97, 43),      S(105, 70),     S(108, 107),    S(114, 109),    S(71, 89),      S(47, 54),      S(32, 27),      //
    S(69, 12),      S(109, -3),     S(95, 61),      S(91, 79),      S(72, 82),      S(75, 68),      S(59, 17),      S(15, 31),      //
    S(11, -33),     S(38, -9),      S(38, 33),      S(51, 58),      S(46, 55),      S(18, 33),      S(10, -2),      S(-32, -34),    //
    S(27, -24),     S(37, -26),     S(15, -14),     S(23, 10),      S(20, 8),       S(-5, -40),     S(-30, -7),     S(-43, -102),   //
    S(-45, -84),    S(6, -25),      S(25, -51),     S(41, -44),     S(24, -23),     S(-11, -49),    S(-22, -32),    S(-66, -126),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-148, 74),    S(-167, 107),   S(-456, 175),   S(-307, 111),   S(-318, 129),   S(-340, 126),   S(-230, 114),   S(-124, 53),    //
    S(-44, -4),     S(-81, 84),     S(-52, 42),     S(-109, 75),    S(-87, 64),     S(-55, 45),     S(-18, 34),     S(-55, 13),     //
    S(22, 24),      S(10, 54),      S(25, 62),      S(18, 57),      S(4, 44),       S(13, 39),      S(12, 36),      S(6, 16),       //
    S(11, 1),       S(47, 15),      S(59, 40),      S(69, 73),      S(103, 53),     S(39, 30),      S(43, -6),      S(-6, 3),       //
    S(44, -52),     S(54, -9),      S(76, 18),      S(91, 41),      S(73, 52),      S(63, 35),      S(20, 10),      S(11, -26),     //
    S(63, -36),     S(84, -21),     S(99, 11),      S(66, 34),      S(63, 13),      S(61, 23),      S(67, 3),       S(11, -4),      //
    S(33, -78),     S(112, -54),    S(74, -29),     S(51, -7),      S(35, -4),      S(47, -37),     S(46, -48),     S(43, -52),     //
    S(54, -72),     S(35, -37),     S(32, -3),      S(49, -30),     S(35, -18),     S(40, 19),      S(49, -13),     S(44, -47),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(186, 100),    S(233, 98),     S(182, 119),    S(180, 87),     S(223, 71),     S(178, 96),     S(201, 101),    S(177, 116),    //
    S(81, 171),     S(165, 162),    S(224, 127),    S(168, 135),    S(204, 129),    S(166, 147),    S(115, 174),    S(97, 178),     //
    S(47, 168),     S(179, 125),    S(208, 101),    S(186, 92),     S(183, 113),    S(131, 149),    S(134, 149),    S(67, 191),     //
    S(23, 131),     S(105, 135),    S(131, 108),    S(110, 115),    S(142, 101),    S(95, 143),     S(99, 138),     S(25, 169),     //
    S(4, 49),       S(81, 58),      S(69, 76),      S(40, 80),      S(48, 93),      S(38, 119),     S(13, 114),     S(-3, 110),     //
    S(21, -36),     S(92, -9),      S(80, 10),      S(59, 11),      S(69, 18),      S(37, 55),      S(46, 30),      S(5, 40),       //
    S(-73, -30),    S(64, -67),     S(66, -39),     S(54, -12),     S(57, -8),      S(45, 4),       S(35, -10),     S(4, 5),        //
    S(-10, -48),    S(15, -26),     S(71, -37),     S(81, -39),     S(81, -28),     S(61, -8),      S(57, -14),     S(37, -7),      //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(52, 167),     S(143, 97),     S(71, 196),     S(15, 288),     S(53, 237),     S(55, 209),     S(75, 127),     S(0, 203),      //
    S(51, 192),     S(59, 290),     S(32, 310),     S(-96, 390),    S(-57, 384),    S(-9, 329),     S(39, 207),     S(17, 206),     //
    S(16, 238),     S(41, 295),     S(20, 340),     S(-24, 368),    S(-15, 372),    S(38, 283),     S(56, 216),     S(63, 131),     //
    S(34, 132),     S(37, 216),     S(-12, 295),    S(-24, 357),    S(-12, 374),    S(21, 256),     S(62, 177),     S(35, 154),     //
    S(36, 95),      S(23, 157),     S(0, 223),      S(-20, 284),    S(-6, 312),     S(8, 263),      S(28, 176),     S(49, 100),     //
    S(31, 18),      S(45, 65),      S(41, 144),     S(23, 153),     S(26, 179),     S(29, 212),     S(46, 148),     S(39, 97),      //
    S(4, -98),      S(20, -45),     S(27, 6),       S(53, 35),      S(46, 83),      S(46, 63),      S(19, 101),     S(36, 71),      //
    S(-9, -92),     S(14, -221),    S(39, -211),    S(55, -107),    S(50, -6),      S(56, -54),     S(50, -19),     S(25, 10),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-65, -284),   S(271, 292),    S(153, 287),    S(-31, 204),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(199, 20),     S(261, 250),    S(97, 306),     S(-5, 210),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(118, 134),    S(157, 235),    S(48, 280),     S(-35, 215),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-44, 73),     S(52, 164),     S(-91, 240),    S(-128, 248),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-107, -13),   S(-34, 74),     S(-124, 150),   S(-222, 221),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-15, -69),    S(37, -3),      S(-72, 85),     S(-139, 148),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(43, -126),    S(51, -52),     S(-28, 7),      S(-98, 65),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-1, -227),    S(24, -122),    S(-60, -72),    S(-57, -85),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-83, -491);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -6), S(15, -24), S(14, -2), S(6, -9), S(1, -26),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(3, -9), S(6, -22), S(4, -12), S(4, -1), S(4, 5),
};

inline const PParam KS_FLANK_ATTACK = S(4, -8);
inline const PParam KS_FLANK_DEFENSE = S(-5, 1);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -5);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, -7);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, 25), S(-5, 28), S(-3, -46), S(8, -30), S(8, -22), S(-10, 14), S(-20, 15), }},
  {{ S(-2, 116), S(-20, -238), S(-20, -49), S(-8, 30), S(-9, 54), S(-22, 89), S(-36, 90), }},
  {{ S(-3, 99), S(-8, -207), S(-8, 91), S(-2, 69), S(-3, 63), S(-13, 70), S(-38, 81), }},
  {{ S(5, 110), S(-10, 75), S(-1, 84), S(6, 67), S(3, 56), S(7, 73), S(1, 76), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(15, 106), S(-8, 67), S(-10, 88), S(-1, 101), S(14, 69),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(3, 132), S(-51, -346), S(-11, -281), S(8, -316), S(-5, 56), S(-10, 79), S(-9, 82), }},
  {{ S(9, 50), S(-29, -460), S(-9, -163), S(-3, -46), S(-2, 8), S(-9, 32), S(1, 28), }},
  {{ S(0, 72), S(-12, -313), S(5, -13), S(3, 2), S(-4, 35), S(-9, 58), S(-4, 52), }},
  {{ S(-1, 64), S(-2, -98), S(7, -14), S(6, 26), S(-6, 43), S(-13, 53), S(-8, 55), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1051, 284, -14, 72
);

inline VParam WINNABLE_PAWNS = V(-23);
inline VParam WINNABLE_SYM = V(125);
inline VParam WINNABLE_ASYM = V(108);
inline VParam WINNABLE_PAWN_ENDGAME = V(193);
inline VParam WINNABLE_BIAS = V(-490);

// Epoch duration: 4.71661s
// clang-format on
}  // namespace Clockwork
