#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(202, 489);
inline const PParam KNIGHT_MAT = S(855, 1507);
inline const PParam BISHOP_MAT = S(898, 1565);
inline const PParam ROOK_MAT   = S(1097, 2357);
inline const PParam QUEEN_MAT  = S(2569, 3548);

inline const PParam TEMPO_VAL  = S(72, 57);

inline const PParam BISHOP_XRAY_PAWNS   = S(-14, -2);
inline const PParam BISHOP_PAIR_VAL   = S(60, 239);
inline const PParam ROOK_OPEN_VAL     = S(113, -6);
inline const PParam ROOK_SEMIOPEN_VAL = S(48, 8);
inline const PParam MINOR_BEHIND_PAWN = S(16, 44);
inline const PParam RESTRICTED_SQUARES = S(19, 8);
inline const PParam SPACE_VAL = S(81, -76);

inline const PParam DOUBLED_PAWN_VAL = S(-18, -85);
inline const PParam ISOLATED_PAWN_VAL = S(-12, -42);

inline const PParam POTENTIAL_CHECKER_VAL = S(-46, -41);
inline const PParam OUTPOST_KNIGHT_VAL    = S(50, 44);
inline const PParam OUTPOST_BISHOP_VAL    = S(44, 36);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(38, 54);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(41, 4);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(17, 70);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(59, -44);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(18, -9), S(39, 25), S(52, 72), S(121, 208), S(437, 379), S(565, 639),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(54, 22), S(42, 21), S(42, 67), S(88, 178), S(406, 113),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-91, -248), S(-90, -208), S(-58, -81), S(-14, 43), S(76, 219), S(216, 432),
};
inline const std::array<PParam, 6> PASSED_CLEAR_STOPPERS = {
    S(-72, 50), S(-91, 40), S(-48, 95), S(-91, 219), S(-66, 334), S(43, 228),
};
inline const std::array<PParam, 6> PASSED_CLEAR_FORWARD = {
    S(-2, 56), S(-13, 54), S(-60, 105), S(-20, 121), S(78, 146), S(40, 194),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(23, -38), S(20, 2), S(14, 36), S(24, 95), S(72, 261), S(311, 256),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(7, -29), S(-2, 14), S(-12, -18), S(-8, -60), S(-15, -171), S(-180, -459),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(24, 221), S(9, 171), S(2, 97), S(14, 57), S(26, 62), S(61, 56), S(69, 29),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-120, -60), S(17, -10), S(2, 97), S(30, 142), S(43, 180), S(54, 187), S(39, 166),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-113, -359), S(-38, -96), S(-7, 22), S(13, 74), S(36, 111), S(50, 151), S(69, 157), S(87, 162), S(107, 116),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-88, -284), S(-25, -65), S(31, 15), S(50, 82), S(68, 128), S(79, 160), S(85, 184), S(87, 205), S(91, 213), S(97, 220), S(109, 209), S(115, 197), S(123, 197), S(82, 164),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(90, -104), S(8, 105), S(29, 139), S(45, 157), S(56, 176), S(60, 190), S(64, 207), S(69, 214), S(74, 226), S(79, 234), S(83, 241), S(82, 249), S(84, 254), S(92, 238), S(106, 205),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-177, -97), S(-23, 15), S(1, 175), S(20, 330), S(39, 371), S(48, 422), S(53, 468), S(55, 494), S(58, 520), S(61, 539), S(65, 552), S(71, 556), S(73, 564), S(78, 563), S(79, 568), S(81, 567), S(77,570), S(77, 561), S(78, 558), S(88, 545), S(73, 541), S(97, 500), S(64, 532), S(26, 524), S(3, 505), S(-32, 531), S(-13, 486), S(-6, 431),
};

inline const PParam PAWN_THREAT_KNIGHT = S(202, 160);
inline const PParam PAWN_THREAT_BISHOP = S(170, 231);
inline const PParam PAWN_THREAT_ROOK   = S(194, 147);
inline const PParam PAWN_THREAT_QUEEN  = S(159, 23);

inline const std::array<std::array<PParam, 5>, 2> MINOR_THREAT = {{
  {{ S(9, 54), S(98, 96), S(104, 122), S(183, 84), S(174, 4), }},
  {{ S(6, 57), S(89, 87), S(103, 117), S(216, 590), S(150, -25), }},
}};
inline const std::array<std::array<PParam, 5>, 2> ROOK_THREAT = {{
  {{ S(9, 64), S(45, 114), S(64, 109), S(16, 20), S(164, -82), }},
  {{ S(-0, 52), S(63, 80), S(103, 64), S(13, 16), S(425, 610), }},
}};
inline const PParam KING_THREAT  = S(-1, 151);
inline const PParam HANGING_PAWN  = S(30, 94);
inline const PParam HANGING_NON_PAWN  = S(71, 31);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(22, -16), S(95, -75),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(50, 34), S(210, -235),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(40, 8), S(126, -108),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -18), S(4, -16), S(0, -22), S(-6, -29), S(-12, -36), S(-18, -40), S(-21, -49), S(-27, -45), S(-26, -92),
};

inline const PParam ROOK_LINEUP = S(17, 86);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(195, 276),    S(63, 400),     S(72, 387),     S(174, 271),    S(182, 194),    S(153, 219),    S(136, 244),    S(225, 199),    //
    S(55, 68),      S(52, 114),     S(33, 72),      S(37, 15),      S(28, -24),     S(7, -1),       S(9, 41),       S(-15, 65),     //
    S(37, -1),      S(9, 7),        S(28, -22),     S(-1, -35),     S(-17, -49),    S(-22, -45),    S(-46, -31),    S(-44, 9),      //
    S(-9, -60),     S(-37, -31),    S(-7, -45),     S(-26, -55),    S(-49, -65),    S(-51, -55),    S(-90, -44),    S(-86, -38),    //
    S(-21, -94),    S(9, -87),      S(-3, -25),     S(-19, -33),    S(-43, -44),    S(-65, -48),    S(-94, -50),    S(-100, -53),   //
    S(3, -90),      S(79, -75),     S(83, -29),     S(31, 2),       S(2, -23),      S(-24, -44),    S(-62, -37),    S(-76, -36),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-293, -444),  S(-242, 57),    S(-290, 132),   S(27, 51),      S(-72, 60),     S(-299, 133),   S(-331, 117),   S(-385, -357),  //
    S(4, 17),       S(-7, 52),      S(82, 33),      S(81, 90),      S(95, 68),      S(49, 45),      S(-8, 39),      S(-52, 46),     //
    S(35, -5),      S(43, 45),      S(50, 72),      S(72, 94),      S(56, 91),      S(12, 83),      S(-3, 58),      S(-44, 26),     //
    S(82, 50),      S(87, 65),      S(89, 92),      S(98, 128),     S(100, 120),    S(66, 89),      S(45, 49),      S(16, 61),      //
    S(69, 39),      S(90, 22),      S(98, 62),      S(83, 104),     S(78, 100),     S(60, 86),      S(47, 39),      S(13, 37),      //
    S(10, -27),     S(41, -17),     S(46, 34),      S(56, 71),      S(50, 74),      S(23, 32),      S(6, 0),        S(-36, -26),    //
    S(26, -2),      S(21, 23),      S(19, -7),      S(30, 25),      S(24, 25),      S(-5, -24),     S(-48, 24),     S(-67, -62),    //
    S(-65, -68),    S(-4, -12),     S(14, -18),     S(31, -7),      S(15, 1),       S(-27, -28),    S(-42, 7),      S(-101, -107),  //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-120, 20),    S(-225, 142),   S(-405, 214),   S(-272, 135),   S(-279, 154),   S(-260, 165),   S(-193, 133),   S(-152, 126),   //
    S(-37, 15),     S(-77, 125),    S(-57, 102),    S(-64, 98),     S(-59, 104),    S(-37, 76),     S(-21, 72),     S(-62, 60),     //
    S(40, 44),      S(18, 81),      S(26, 95),      S(21, 86),      S(23, 67),      S(13, 83),      S(-5, 78),      S(13, 24),      //
    S(8, 21),       S(45, 54),      S(59, 60),      S(79, 85),      S(92, 80),      S(35, 57),      S(32, 39),      S(-9, 31),      //
    S(33, -12),     S(33, 38),      S(69, 44),      S(84, 63),      S(71, 71),      S(56, 61),      S(9, 43),       S(-3, 0),       //
    S(47, 8),       S(75, 4),       S(87, 33),      S(58, 60),      S(52, 51),      S(48, 47),      S(51, 28),      S(14, 1),       //
    S(21, -38),     S(109, -35),    S(54, 16),      S(37, 31),      S(23, 29),      S(37, -14),     S(39, -27),     S(26, -12),     //
    S(25, -35),     S(13, 6),       S(16, 28),      S(29, 6),       S(13, 12),      S(26, 46),      S(31, 16),      S(24, -27),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(162, 174),    S(183, 193),    S(148, 223),    S(135, 188),    S(182, 160),    S(135, 183),    S(153, 198),    S(147, 186),    //
    S(91, 210),     S(131, 217),    S(184, 174),    S(136, 172),    S(173, 162),    S(140, 182),    S(86, 219),     S(80, 219),     //
    S(67, 194),     S(174, 142),    S(214, 118),    S(174, 107),    S(174, 130),    S(112, 170),    S(92, 200),     S(51, 232),     //
    S(29, 170),     S(89, 182),     S(99, 161),     S(86, 154),     S(109, 141),    S(75, 180),     S(58, 202),     S(11, 220),     //
    S(2, 106),      S(56, 118),     S(42, 151),     S(10, 155),     S(23, 155),     S(14, 178),     S(-4, 183),     S(-30, 186),    //
    S(5, 36),       S(80, 51),      S(60, 83),      S(36, 101),     S(43, 109),     S(27, 126),     S(22, 109),     S(-22, 123),    //
    S(-70, 29),     S(52, -12),     S(55, 30),      S(41, 69),      S(46, 66),      S(30, 80),      S(21, 66),      S(-9, 58),      //
    S(-9, -3),      S(11, 33),      S(74, 24),      S(83, 24),      S(85, 31),      S(60, 55),      S(56, 45),      S(36, 49),      //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(108, 218),    S(171, 208),    S(79, 353),     S(44, 409),     S(49, 419),     S(105, 307),    S(73, 263),     S(30, 279),     //
    S(71, 256),     S(71, 317),     S(43, 386),     S(-56, 474),    S(-19, 462),    S(39, 330),     S(61, 260),     S(49, 221),     //
    S(50, 261),     S(93, 290),     S(52, 375),     S(21, 399),     S(32, 411),     S(63, 328),     S(98, 221),     S(78, 190),     //
    S(41, 219),     S(47, 288),     S(5, 355),      S(7, 406),      S(26, 401),     S(28, 308),     S(69, 234),     S(51, 194),     //
    S(24, 208),     S(27, 209),     S(23, 257),     S(-4, 336),     S(-1, 370),     S(23, 303),     S(24, 248),     S(42, 166),     //
    S(20, 117),     S(54, 100),     S(46, 187),     S(18, 226),     S(28, 237),     S(26, 271),     S(45, 206),     S(29, 182),     //
    S(-17, -34),    S(21, -48),     S(26, 30),      S(49, 80),      S(45, 125),     S(46, 101),     S(23, 135),     S(36, 103),     //
    S(-47, -0),     S(-1, -191),    S(23, -166),    S(33, -59),     S(48, 10),      S(44, 5),       S(40, 16),      S(7, 80),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(124, -434),   S(478, 110),    S(432, 132),    S(65, 189),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(328, -167),   S(326, 185),    S(227, 172),    S(23, 126),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(185, 28),     S(202, 160),    S(74, 183),     S(-92, 166),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(30, 2),       S(106, 93),     S(-49, 162),    S(-166, 194),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-112, -4),    S(-38, 75),     S(-97, 141),    S(-206, 191),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-58, -28),    S(1, 37),       S(-80, 119),    S(-140, 158),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(31, -96),     S(32, -13),     S(-33, 52),     S(-114, 112),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-17, -203),   S(9, -91),      S(-71, -23),    S(-77, -41),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-88, -434);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -1), S(12, 15), S(12, 7), S(5, 3), S(3, -7),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(5, -6), S(6, 5), S(3, 2), S(3, 0), S(4, -1),
};

inline const PParam KS_FLANK_ATTACK = S(4, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -1);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(4, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 1);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(16, -14), S(-11, 7), S(-6, -9), S(5, -13), S(13, -30), S(7, -53), S(-5, -51), }},
  {{ S(1, 10), S(-26, 4), S(-21, 13), S(-13, 22), S(-9, 13), S(-11, -13), S(-24, -22), }},
  {{ S(-4, 3), S(-13, -5), S(-15, 22), S(-8, 22), S(-7, 10), S(-7, -14), S(-21, -54), }},
  {{ S(5, 14), S(-13, 22), S(-9, 47), S(-4, 51), S(-3, 46), S(1, 27), S(14, -16), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(10, 28), S(-9, 6), S(-9, 12), S(-8, 34), S(3, 52),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 16), S(-55, -118), S(-12, -41), S(1, 2), S(-4, 7), S(-7, 13), S(-7, 13), }},
  {{ S(10, 1), S(-31, -110), S(-7, -41), S(-4, -1), S(-3, -1), S(-12, 5), S(-0, 5), }},
  {{ S(0, 14), S(-5, -83), S(8, -10), S(2, 8), S(-4, 15), S(-11, 22), S(-7, 24), }},
  {{ S(1, 1), S(2, -69), S(0, 25), S(-4, 35), S(-8, 14), S(-15, 11), S(-11, 19), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1119, 861, -24, 3
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(89);
inline VParam WINNABLE_PAWN_ENDGAME = V(45);
inline VParam WINNABLE_BIAS = V(-379);

// Epoch duration: 3.83668s
// clang-format on
}  // namespace Clockwork
