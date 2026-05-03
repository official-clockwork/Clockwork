#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(183, 436);
inline const PParam KNIGHT_MAT = S(834, 1255);
inline const PParam BISHOP_MAT = S(873, 1317);
inline const PParam ROOK_MAT   = S(1040, 2100);
inline const PParam QUEEN_MAT  = S(2541, 3255);
inline const PParam TEMPO_VAL  = S(31, 30);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, 14);
inline const PParam BISHOP_PAIR_VAL   = S(53, 220);
inline const PParam ROOK_OPEN_VAL     = S(114, -12);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 31);
inline const PParam MINOR_BEHIND_PAWN = S(14, 40);
inline const PParam RESTRICTED_SQUARES = S(-23, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-13, -77);
inline const PParam ISOLATED_PAWN_VAL = S(-17, -24);

inline const PParam POTENTIAL_CHECKER_VAL = S(-50, -23);
inline const PParam OUTPOST_KNIGHT_VAL    = S(52, 55);
inline const PParam OUTPOST_BISHOP_VAL    = S(57, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 5);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(44, -15);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 52);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(61, -41);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(16, 3), S(29, 35), S(58, 59), S(138, 167), S(365, 256), S(470, 509),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(44, 42), S(44, 34), S(60, 61), S(117, 163), S(413, 90),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-103, -217), S(-104, -185), S(-78, -64), S(-26, 49), S(61, 210), S(251, 299),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(27, -41), S(23, -2), S(16, 33), S(5, 119), S(76, 235), S(188, 378),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(16, -26), S(5, 18), S(0, -17), S(1, -47), S(-9, -136), S(-250, -240),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(16, 205), S(-8, 174), S(-0, 95), S(8, 56), S(15, 57), S(51, 53), S(43, 36),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-264, -45), S(-6, 37), S(-0, 98), S(37, 129), S(57, 153), S(68, 158), S(45, 158),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-99, -298), S(-38, -106), S(-9, -4), S(16, 34), S(46, 54), S(61, 88), S(81, 88), S(101, 98), S(133, 26),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-67, -277), S(-21, -101), S(30, -26), S(53, 24), S(74, 60), S(86, 88), S(91, 109), S(99, 121), S(101, 137), S(113, 127), S(117, 127), S(153, 78), S(158, 71), S(181, 22),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(104, -145), S(24, 26), S(45, 56), S(61, 67), S(71, 82), S(76, 95), S(81, 107), S(88, 110), S(93, 122), S(100, 125), S(108, 129), S(111, 136), S(113, 136), S(120, 112), S(189, 8),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-162, -72), S(-32, 19), S(-7, 142), S(13, 256), S(27, 301), S(35, 346), S(41, 379), S(49, 388), S(52, 412), S(55, 428), S(60, 433), S(63, 444), S(68, 441), S(68, 447), S(70, 444), S(66, 446), S(65, 442), S(70, 427), S(73, 415), S(77, 402), S(74, 384), S(91, 339), S(71, 345), S(49, 299), S(19, 297), S(3, 267), S(-48, 283), S(-37, 218),
};

inline const PParam PAWN_THREAT_KNIGHT = S(203, 103);
inline const PParam PAWN_THREAT_BISHOP = S(191, 152);
inline const PParam PAWN_THREAT_ROOK   = S(202, 128);
inline const PParam PAWN_THREAT_QUEEN  = S(175, 4);

inline const PParam KNIGHT_THREAT_BISHOP = S(108, 112);
inline const PParam KNIGHT_THREAT_ROOK   = S(227, 78);
inline const PParam KNIGHT_THREAT_QUEEN  = S(151, 17);

inline const PParam BISHOP_THREAT_KNIGHT = S(97, 71);
inline const PParam BISHOP_THREAT_ROOK   = S(210, 135);
inline const PParam BISHOP_THREAT_QUEEN  = S(165, 129);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(8, -26), S(0, -8), S(-3, -17), S(-6, -27), S(-11, -37), S(-14, -44), S(-16, -57), S(-21, -59), S(-35, -56),
};

inline const PParam ROOK_LINEUP = S(15, 66);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(208, 225),    S(52, 279),     S(195, 212),    S(178, 181),    S(221, 108),    S(118, 199),    S(54, 250),     S(193, 191),    //
    S(44, 29),      S(73, 43),      S(77, 12),      S(56, -52),     S(49, -84),     S(28, -34),     S(9, 22),       S(-22, 50),     //
    S(26, -25),     S(3, -18),      S(34, -60),     S(21, -84),     S(16, -100),    S(-6, -72),     S(-51, -25),    S(-48, 3),      //
    S(-7, -77),     S(-25, -49),    S(2, -73),      S(-11, -80),    S(-32, -83),    S(-38, -66),    S(-93, -31),    S(-83, -37),    //
    S(-6, -109),    S(28, -110),    S(11, -54),     S(-4, -58),     S(-32, -67),    S(-49, -64),    S(-81, -50),    S(-80, -56),    //
    S(16, -107),    S(95, -109),    S(72, -61),     S(31, -26),     S(2, -45),      S(-20, -58),    S(-50, -42),    S(-62, -41),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-298, -394),  S(-283, 45),    S(-234, -186),  S(-31, 45),     S(-97, 39),     S(-233, 35),    S(-401, 108),   S(-375, -270),  //
    S(-44, -13),    S(-27, 33),     S(86, -4),      S(86, 30),      S(90, 25),      S(22, 31),      S(-42, 35),     S(-64, 8),      //
    S(26, -19),     S(39, 11),      S(74, 64),      S(95, 59),      S(64, 84),      S(16, 83),      S(6, 20),       S(-41, 12),     //
    S(90, 10),      S(99, 42),      S(107, 71),     S(108, 108),    S(115, 109),    S(72, 88),      S(47, 53),      S(32, 26),      //
    S(69, 13),      S(109, -3),     S(96, 61),      S(91, 80),      S(72, 82),      S(75, 69),      S(59, 18),      S(14, 31),      //
    S(11, -33),     S(38, -8),      S(39, 33),      S(51, 59),      S(46, 56),      S(17, 33),      S(8, -0),       S(-31, -34),    //
    S(27, -25),     S(37, -27),     S(15, -13),     S(23, 10),      S(20, 9),       S(-5, -39),     S(-31, -7),     S(-43, -102),   //
    S(-45, -85),    S(7, -25),      S(26, -51),     S(41, -44),     S(25, -24),     S(-9, -51),     S(-21, -33),    S(-64, -129),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-149, 72),    S(-170, 104),   S(-459, 176),   S(-309, 110),   S(-315, 125),   S(-342, 125),   S(-236, 118),   S(-125, 54),    //
    S(-48, -3),     S(-85, 85),     S(-57, 46),     S(-111, 75),    S(-89, 65),     S(-59, 47),     S(-22, 37),     S(-59, 14),     //
    S(21, 24),      S(8, 55),       S(23, 63),      S(17, 55),      S(0, 47),       S(10, 40),      S(10, 35),      S(4, 14),       //
    S(9, 2),        S(45, 15),      S(59, 37),      S(68, 71),      S(102, 52),     S(39, 28),      S(39, -6),      S(-8, 2),       //
    S(42, -52),     S(51, -8),      S(75, 17),      S(91, 39),      S(72, 50),      S(61, 32),      S(17, 9),       S(9, -28),      //
    S(61, -37),     S(83, -22),     S(98, 10),      S(64, 33),      S(60, 13),      S(59, 22),      S(63, 2),       S(9, -4),       //
    S(31, -80),     S(111, -56),    S(72, -30),     S(49, -8),      S(34, -4),      S(45, -38),     S(44, -49),     S(41, -53),     //
    S(52, -74),     S(34, -39),     S(30, -5),      S(48, -32),     S(34, -19),     S(39, 18),      S(47, -14),     S(44, -49),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(186, 100),    S(236, 96),     S(185, 119),    S(186, 84),     S(227, 68),     S(182, 93),     S(202, 99),     S(176, 117),    //
    S(82, 170),     S(168, 160),    S(226, 128),    S(172, 135),    S(207, 126),    S(169, 145),    S(116, 172),    S(97, 178),     //
    S(48, 167),     S(181, 124),    S(213, 100),    S(186, 94),     S(184, 113),    S(131, 148),    S(135, 149),    S(68, 190),     //
    S(24, 131),     S(107, 136),    S(135, 107),    S(112, 115),    S(143, 101),    S(96, 142),     S(99, 139),     S(25, 170),     //
    S(3, 53),       S(81, 60),      S(70, 77),      S(41, 82),      S(48, 93),      S(37, 120),     S(13, 115),     S(-4, 111),     //
    S(19, -32),     S(93, -7),      S(79, 13),      S(58, 14),      S(68, 20),      S(35, 58),      S(44, 32),      S(4, 43),       //
    S(-75, -25),    S(64, -65),     S(66, -36),     S(54, -10),     S(56, -5),      S(43, 6),       S(34, -8),      S(4, 6),        //
    S(-11, -45),    S(15, -24),     S(71, -34),     S(80, -36),     S(80, -26),     S(60, -6),      S(56, -12),     S(37, -5),      //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(50, 165),     S(143, 95),     S(70, 195),     S(18, 281),     S(51, 237),     S(49, 212),     S(71, 127),     S(-1, 198),     //
    S(50, 193),     S(59, 291),     S(32, 312),     S(-93, 393),    S(-56, 388),    S(-10, 332),    S(38, 206),     S(17, 202),     //
    S(19, 235),     S(42, 297),     S(21, 344),     S(-23, 372),    S(-16, 377),    S(37, 286),     S(55, 215),     S(62, 130),     //
    S(36, 132),     S(37, 220),     S(-11, 299),    S(-24, 361),    S(-14, 380),    S(20, 257),     S(62, 175),     S(35, 151),     //
    S(36, 98),      S(24, 158),     S(2, 224),      S(-21, 289),    S(-8, 316),     S(6, 264),      S(29, 171),     S(48, 98),      //
    S(30, 19),      S(45, 67),      S(40, 148),     S(22, 155),     S(24, 181),     S(27, 212),     S(44, 147),     S(37, 98),      //
    S(2, -96),      S(20, -44),     S(27, 6),       S(52, 36),      S(45, 84),      S(45, 61),      S(17, 99),      S(34, 73),      //
    S(-9, -93),     S(15, -225),    S(40, -215),    S(56, -109),    S(51, -9),      S(56, -56),     S(51, -24),     S(26, 4),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-56, -294),   S(285, 285),    S(162, 283),    S(-7, 192),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(225, 2),      S(274, 236),    S(116, 286),    S(25, 191),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(138, 112),    S(177, 218),    S(72, 257),     S(-3, 195),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-27, 57),     S(67, 150),     S(-62, 218),    S(-95, 232),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-107, -12),   S(-30, 72),     S(-106, 143),   S(-191, 209),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-28, -59),    S(27, 6),       S(-71, 88),     S(-125, 144),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(32, -112),    S(40, -39),     S(-33, 16),     S(-94, 68),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-12, -212),   S(13, -108),    S(-67, -63),    S(-58, -78),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-84, -536);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(10, -19), S(14, -26), S(14, -3), S(5, -9), S(1, -29),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(3, -12), S(6, -23), S(4, -12), S(4, -0), S(4, 7),
};

inline const PParam KS_FLANK_ATTACK = S(4, -10);
inline const PParam KS_FLANK_DEFENSE = S(-4, 1);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -4);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, -7);

inline const PParam KS_TEMPO  = S(11, -15);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(16, 45), S(-5, 53), S(-4, -15), S(8, -15), S(6, -3), S(-5, -4), S(-4, 54), }},
  {{ S(-4, 158), S(-21, -228), S(-22, -4), S(-10, 56), S(-10, 79), S(-21, -11), S(-20, -216), }},
  {{ S(-5, 137), S(-10, -212), S(-11, 129), S(-6, 110), S(-5, 109), S(-11, 135), S(-9, -193), }},
  {{ S(6, 123), S(-11, 84), S(-0, 94), S(6, 78), S(4, 80), S(-2, 92), S(-7, 84), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(7, 64), S(-12, 44), S(-12, 72), S(-4, 79), S(2, 34),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(1, 115), S(-10, 41), S(-11, 46), S(-8, 41), S(-8, 31), S(-10, 44), S(-10, 47), }},
  {{ S(7, 11), S(-1, -25), S(-10, -15), S(-5, -50), S(-6, -46), S(-9, -12), S(-1, -18), }},
  {{ S(-2, 58), S(-5, 34), S(-8, 39), S(-6, 33), S(-5, 17), S(-9, 36), S(-5, 26), }},
  {{ S(-3, 55), S(-8, 37), S(-12, 38), S(-7, 42), S(-6, 41), S(-12, 32), S(-10, 38), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1136, 269, -17, 98
);

inline VParam WINNABLE_PAWNS = V(-24);
inline VParam WINNABLE_SYM = V(125);
inline VParam WINNABLE_ASYM = V(108);
inline VParam WINNABLE_PAWN_ENDGAME = V(191);
inline VParam WINNABLE_BIAS = V(-484);

// Epoch duration: 5.51875s
// clang-format on
}  // namespace Clockwork
