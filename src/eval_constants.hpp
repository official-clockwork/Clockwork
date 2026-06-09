#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(197, 501);
inline const PParam KNIGHT_MAT = S(824, 1540);
inline const PParam BISHOP_MAT = S(865, 1627);
inline const PParam ROOK_MAT   = S(1032, 2550);
inline const PParam QUEEN_MAT  = S(2451, 3871);

inline const PParam TEMPO_VAL  = S(69, 50);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -4);
inline const PParam BISHOP_PAIR_VAL   = S(64, 230);
inline const PParam ROOK_OPEN_VAL     = S(109, -2);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 13);
inline const PParam MINOR_BEHIND_PAWN = S(15, 39);
inline const PParam RESTRICTED_SQUARES = S(-20, -4);

inline const PParam DOUBLED_PAWN_VAL = S(-20, -82);
inline const PParam ISOLATED_PAWN_VAL = S(-14, -36);

inline const PParam POTENTIAL_CHECKER_VAL = S(-46, -40);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 42);
inline const PParam OUTPOST_BISHOP_VAL    = S(42, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(38, 48);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(41, 2);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(21, 59);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(62, -51);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -1), S(31, 36), S(61, 62), S(128, 194), S(441, 264), S(568, 549),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(46, 34), S(38, 25), S(56, 53), S(99, 170), S(409, 89),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-88, -270), S(-88, -234), S(-66, -96), S(-27, 42), S(69, 238), S(214, 411),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(25, -40), S(25, -1), S(12, 41), S(11, 121), S(77, 247), S(106, 455),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -38), S(2, 9), S(-4, -23), S(-1, -59), S(-10, -166), S(-169, -335),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(14, 239), S(1, 191), S(-2, 115), S(9, 73), S(20, 76), S(59, 74), S(55, 49),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-137, -60), S(17, 4), S(2, 108), S(27, 158), S(40, 194), S(52, 200), S(32, 191),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-88, -373), S(-27, -134), S(1, -16), S(25, 31), S(53, 62), S(68, 98), S(86, 106), S(105, 115), S(124, 69),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-62, -284), S(-14, -104), S(34, -29), S(57, 21), S(78, 62), S(89, 91), S(94, 113), S(97, 133), S(101, 146), S(107, 153), S(113, 149), S(133, 131), S(134, 132), S(116, 104),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(108, -177), S(24, 10), S(44, 43), S(60, 60), S(71, 77), S(74, 91), S(79, 106), S(85, 111), S(89, 124), S(95, 130), S(101, 135), S(102, 142), S(105, 147), S(107, 137), S(132, 93),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-206, -159), S(-51, 6), S(-26, 132), S(-4, 256), S(13, 295), S(19, 348), S(26, 381), S(32, 396), S(36, 420), S(40, 437), S(45, 445), S(49, 453), S(52, 459), S(54, 465), S(55, 468), S(54, 470), S(51, 470), S(56, 461), S(56, 460), S(59, 452), S(55, 436), S(59, 418), S(28, 444), S(-17, 432), S(-46, 434), S(-75, 451), S(-118, 463), S(-85, 391),
};

inline const PParam PAWN_THREAT_KNIGHT = S(194, 147);
inline const PParam PAWN_THREAT_BISHOP = S(170, 212);
inline const PParam PAWN_THREAT_ROOK   = S(194, 137);
inline const PParam PAWN_THREAT_QUEEN  = S(155, 17);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(6, 56), S(91, 91), S(102, 124), S(203, 124), S(159, 12), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(7, 56), S(52, 93), S(72, 94), S(21, -6), S(148, -51), S(0, 0),
};
inline const PParam KING_THREAT  = S(-19, 157);
inline const PParam HANGING_PAWN  = S(32, 88);
inline const PParam HANGING_NON_PAWN  = S(69, 22);

inline const std::array<PParam, 2> KNIGHT_ON_QUEEN = {
    S(21, -18), S(27, -2),
};
inline const std::array<PParam, 2> BISHOP_ON_QUEEN = {
    S(40, 46), S(123, -120),
};
inline const std::array<PParam, 2> ROOK_ON_QUEEN = {
    S(36, 12), S(49, -7),
};

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -16), S(4, -14), S(-0, -21), S(-5, -29), S(-11, -37), S(-16, -44), S(-18, -56), S(-24, -60), S(-28, -74),
};

inline const PParam ROOK_LINEUP = S(17, 78);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(188, 277),    S(42, 392),     S(66, 363),     S(147, 259),    S(196, 158),    S(124, 226),    S(88, 269),     S(229, 203),    //
    S(38, 69),      S(51, 110),     S(30, 53),      S(47, -25),     S(39, -57),     S(10, -18),     S(7, 34),       S(-22, 65),     //
    S(34, -7),      S(10, 0),       S(41, -46),     S(27, -69),     S(17, -87),     S(-2, -71),     S(-41, -34),    S(-42, 5),      //
    S(-8, -70),     S(-32, -39),    S(2, -62),      S(-6, -75),     S(-28, -84),    S(-38, -71),    S(-86, -49),    S(-79, -47),    //
    S(-12, -107),   S(21, -105),    S(8, -47),      S(-7, -53),     S(-35, -62),    S(-51, -71),    S(-81, -67),    S(-83, -70),    //
    S(10, -105),    S(85, -95),     S(71, -45),     S(29, -20),     S(-3, -40),     S(-22, -63),    S(-50, -55),    S(-63, -53),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-256, -496),  S(-253, 54),    S(-268, -74),   S(33, 17),      S(-94, 42),     S(-260, 72),    S(-366, 117),   S(-346, -394),  //
    S(-7, -4),      S(-0, 36),      S(95, 20),      S(80, 55),      S(84, 49),      S(53, 23),      S(-27, 30),     S(-30, 18),     //
    S(48, -28),     S(36, 31),      S(69, 48),      S(70, 72),      S(60, 68),      S(20, 56),      S(7, 34),       S(-42, 20),     //
    S(95, 25),      S(89, 51),      S(102, 66),     S(99, 111),     S(104, 111),    S(68, 76),      S(49, 43),      S(37, 37),      //
    S(77, 17),      S(103, 6),      S(97, 52),      S(90, 84),      S(76, 86),      S(72, 67),      S(60, 15),      S(22, 33),      //
    S(18, -36),     S(45, -17),     S(49, 24),      S(61, 59),      S(56, 57),      S(30, 18),      S(15, -7),      S(-25, -31),    //
    S(41, -25),     S(41, -7),      S(27, -11),     S(34, 16),      S(32, 12),      S(5, -37),      S(-29, -0),     S(-38, -88),    //
    S(-42, -71),    S(8, -19),      S(27, -37),     S(44, -24),     S(25, -7),      S(-12, -43),    S(-19, -18),    S(-64, -125),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-126, 28),    S(-177, 113),   S(-421, 185),   S(-256, 87),    S(-290, 141),   S(-295, 151),   S(-209, 128),   S(-121, 72),    //
    S(-30, -10),    S(-75, 97),     S(-36, 60),     S(-66, 69),     S(-66, 76),     S(-34, 47),     S(-7, 43),      S(-43, 25),     //
    S(37, 29),      S(15, 68),      S(33, 77),      S(26, 68),      S(23, 50),      S(19, 54),      S(13, 51),      S(17, 20),      //
    S(16, 4),       S(52, 27),      S(63, 46),      S(75, 65),      S(101, 51),     S(46, 25),      S(51, 0),       S(2, 4),        //
    S(44, -39),     S(47, 9),       S(79, 20),      S(94, 32),      S(77, 49),      S(69, 29),      S(22, 14),      S(19, -28),     //
    S(61, -24),     S(85, -21),     S(98, 10),      S(66, 38),      S(64, 20),      S(60, 23),      S(66, 1),       S(20, -12),     //
    S(31, -74),     S(115, -45),    S(71, -11),     S(49, 6),       S(31, 9),       S(53, -37),     S(50, -44),     S(44, -48),     //
    S(52, -73),     S(21, -7),      S(26, 1),       S(48, -25),     S(31, -14),     S(37, 22),      S(46, -11),     S(43, -50),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(192, 92),     S(223, 108),    S(165, 141),    S(150, 121),    S(217, 70),     S(179, 95),     S(191, 101),    S(167, 106),    //
    S(96, 151),     S(148, 150),    S(205, 114),    S(150, 114),    S(191, 96),     S(153, 120),    S(102, 151),    S(99, 152),     //
    S(63, 145),     S(181, 92),     S(207, 73),     S(184, 55),     S(183, 76),     S(123, 116),    S(125, 125),    S(72, 165),     //
    S(36, 114),     S(97, 120),     S(125, 91),     S(99, 96),      S(128, 80),     S(88, 125),     S(81, 133),     S(25, 159),     //
    S(4, 56),       S(68, 63),      S(61, 79),      S(31, 89),      S(41, 94),      S(29, 120),     S(11, 119),     S(-9, 122),     //
    S(17, -21),     S(84, 5),       S(75, 27),      S(54, 36),      S(64, 43),      S(37, 72),      S(37, 55),      S(-5, 62),      //
    S(-64, -19),    S(63, -57),     S(69, -20),     S(58, 12),      S(61, 11),      S(45, 28),      S(34, 11),      S(4, 19),       //
    S(-4, -41),     S(12, -1),      S(83, -18),     S(93, -21),     S(92, -14),     S(69, 12),      S(66, 1),       S(48, 6),       //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(78, 170),     S(151, 143),    S(61, 284),     S(6, 378),      S(51, 321),     S(79, 239),     S(72, 163),     S(10, 218),     //
    S(60, 196),     S(82, 231),     S(48, 303),     S(-85, 433),    S(-33, 394),    S(12, 298),     S(56, 168),     S(27, 171),     //
    S(18, 246),     S(56, 276),     S(19, 358),     S(-18, 393),    S(4, 371),      S(52, 256),     S(82, 160),     S(61, 119),     //
    S(18, 177),     S(20, 260),     S(-11, 309),    S(-23, 377),    S(-8, 377),     S(22, 245),     S(60, 157),     S(29, 134),     //
    S(10, 169),     S(6, 191),      S(-4, 240),     S(-29, 305),    S(-18, 327),    S(6, 255),      S(9, 191),      S(32, 101),     //
    S(3, 72),       S(31, 87),      S(28, 159),     S(6, 179),      S(14, 188),     S(15, 214),     S(36, 142),     S(15, 119),     //
    S(-27, -65),    S(2, -59),      S(9, 11),       S(33, 48),      S(29, 86),      S(30, 64),      S(5, 94),       S(16, 74),      //
    S(-38, -50),    S(-22, -195),   S(8, -196),     S(23, -87),     S(31, -13),     S(29, -44),     S(27, -23),     S(-2, 25),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(99, -577),    S(482, 4),      S(295, 176),    S(2, 153),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(341, -179),   S(377, 134),    S(207, 179),    S(20, 136),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(187, 43),     S(227, 158),    S(120, 189),    S(-37, 165),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(16, 10),      S(96, 112),     S(-17, 159),    S(-91, 176),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-115, -3),    S(-32, 77),     S(-89, 140),    S(-171, 187),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-53, -31),    S(14, 34),      S(-72, 115),    S(-123, 158),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(22, -83),     S(30, -6),      S(-34, 56),     S(-110, 111),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-19, -199),   S(4, -82),      S(-79, -17),    S(-76, -38),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-86, -435);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -4), S(10, 11), S(10, 5), S(5, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(5, 4), S(2, 2), S(3, -0), S(4, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, 0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(18, -20), S(-7, 5), S(-3, -10), S(8, -17), S(12, -27), S(6, -54), S(-5, -45), }},
  {{ S(2, 2), S(-22, -0), S(-17, 8), S(-10, 16), S(-7, 7), S(-10, -11), S(-20, -24), }},
  {{ S(-3, -2), S(-12, -11), S(-13, 18), S(-8, 17), S(-7, 10), S(-9, -17), S(-25, -44), }},
  {{ S(7, 8), S(-10, 18), S(-7, 39), S(-2, 43), S(-2, 38), S(4, 16), S(8, -14), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(10, 25), S(-5, 3), S(-7, 10), S(-5, 28), S(2, 48),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 11), S(-45, -143), S(-8, -45), S(0, 4), S(-2, 4), S(-5, 11), S(-5, 9), }},
  {{ S(10, -0), S(-24, -130), S(-6, -31), S(-3, 1), S(-0, -3), S(-7, 4), S(2, 3), }},
  {{ S(0, 10), S(-10, -81), S(7, -15), S(1, 9), S(-3, 10), S(-9, 19), S(-5, 20), }},
  {{ S(2, -1), S(3, -61), S(-1, 27), S(-2, 25), S(-5, 9), S(-12, 11), S(-9, 19), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1362, 856, -20, -3
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(101);
inline VParam WINNABLE_ASYM = V(84);
inline VParam WINNABLE_PAWN_ENDGAME = V(105);
inline VParam WINNABLE_BIAS = V(-378);

// Epoch duration: 7.18402s
// clang-format on
}  // namespace Clockwork
