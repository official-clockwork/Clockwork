#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(194, 503);
inline const PParam KNIGHT_MAT = S(826, 1543);
inline const PParam BISHOP_MAT = S(867, 1632);
inline const PParam ROOK_MAT   = S(1028, 2561);
inline const PParam QUEEN_MAT  = S(2449, 3864);

inline const PParam TEMPO_VAL  = S(69, 50);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -4);
inline const PParam BISHOP_PAIR_VAL   = S(65, 230);
inline const PParam ROOK_OPEN_VAL     = S(112, -3);
inline const PParam ROOK_SEMIOPEN_VAL = S(45, 12);
inline const PParam MINOR_BEHIND_PAWN = S(15, 38);
inline const PParam RESTRICTED_SQUARES = S(-22, -3);

inline const PParam DOUBLED_PAWN_VAL = S(-20, -82);
inline const PParam ISOLATED_PAWN_VAL = S(-14, -36);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -41);
inline const PParam OUTPOST_KNIGHT_VAL    = S(53, 41);
inline const PParam OUTPOST_BISHOP_VAL    = S(42, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 49);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(41, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(24, 58);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(61, -49);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -1), S(31, 36), S(61, 63), S(129, 194), S(438, 267), S(565, 548),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(46, 34), S(38, 25), S(55, 54), S(98, 170), S(409, 89),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-88, -264), S(-88, -229), S(-65, -90), S(-26, 48), S(69, 243), S(214, 416),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(25, -39), S(25, -0), S(13, 41), S(11, 121), S(79, 246), S(103, 457),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(16, -38), S(3, 8), S(-5, -22), S(-2, -58), S(-10, -166), S(-168, -335),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(14, 237), S(1, 189), S(-2, 112), S(10, 70), S(20, 73), S(59, 71), S(55, 46),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-137, -64), S(15, 2), S(1, 105), S(25, 156), S(38, 192), S(50, 198), S(32, 186),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-89, -372), S(-28, -133), S(1, -15), S(25, 32), S(54, 62), S(69, 98), S(87, 106), S(106, 113), S(126, 68),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-65, -283), S(-17, -103), S(33, -29), S(57, 20), S(79, 60), S(92, 88), S(99, 110), S(103, 129), S(108, 141), S(116, 147), S(123, 143), S(144, 124), S(146, 125), S(126, 98),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(108, -178), S(24, 10), S(45, 42), S(61, 58), S(71, 76), S(75, 90), S(80, 105), S(87, 110), S(92, 122), S(98, 128), S(105, 132), S(108, 139), S(111, 143), S(114, 133), S(138, 89),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-219, -149), S(-64, 21), S(-39, 146), S(-17, 271), S(-1, 310), S(5, 363), S(12, 396), S(19, 412), S(22, 437), S(25, 455), S(30, 464), S(34, 473), S(36, 480), S(37, 486), S(37, 490), S(34, 495), S(30, 496), S(33, 487), S(32, 487), S(36, 478), S(33, 461), S(39, 440), S(7, 468), S(-34, 453), S(-64, 456), S(-93, 474), S(-133, 480), S(-102, 411),
};

inline const PParam PAWN_THREAT_KNIGHT = S(193, 149);
inline const PParam PAWN_THREAT_BISHOP = S(170, 212);
inline const PParam PAWN_THREAT_ROOK   = S(196, 136);
inline const PParam PAWN_THREAT_QUEEN  = S(150, 23);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(8, 55), S(93, 90), S(103, 124), S(203, 124), S(159, 18), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(9, 55), S(54, 92), S(72, 94), S(20, -6), S(173, -56), S(0, 0),
};
inline const PParam KING_THREAT  = S(-17, 156);
inline const PParam HANGING_PAWN  = S(31, 89);
inline const PParam HANGING_NON_PAWN  = S(67, 23);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(4, -16), S(4, -15), S(-0, -21), S(-6, -29), S(-12, -37), S(-17, -44), S(-19, -56), S(-25, -59), S(-29, -74),
};

inline const PParam ROOK_LINEUP = S(16, 79);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(189, 277),    S(41, 395),     S(66, 365),     S(148, 260),    S(198, 159),    S(126, 226),    S(87, 271),     S(229, 203),    //
    S(40, 70),      S(51, 111),     S(31, 54),      S(48, -24),     S(39, -57),     S(11, -17),     S(7, 35),       S(-21, 65),     //
    S(36, -7),      S(10, 1),       S(41, -45),     S(26, -67),     S(18, -86),     S(-1, -71),     S(-40, -33),    S(-40, 5),      //
    S(-6, -69),     S(-32, -38),    S(2, -61),      S(-7, -73),     S(-29, -83),    S(-38, -70),    S(-86, -49),    S(-77, -47),    //
    S(-10, -107),   S(22, -104),    S(8, -46),      S(-8, -52),     S(-35, -62),    S(-51, -70),    S(-81, -66),    S(-81, -70),    //
    S(12, -105),    S(85, -94),     S(73, -44),     S(29, -18),     S(-3, -38),     S(-22, -62),    S(-49, -54),    S(-61, -53),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-256, -499),  S(-253, 52),    S(-267, -78),   S(35, 14),      S(-92, 39),     S(-261, 70),    S(-366, 116),   S(-347, -395),  //
    S(-6, -6),      S(1, 33),       S(98, 17),      S(84, 51),      S(88, 46),      S(57, 19),      S(-26, 29),     S(-28, 14),     //
    S(48, -30),     S(38, 28),      S(72, 46),      S(75, 67),      S(67, 62),      S(25, 53),      S(11, 30),      S(-41, 17),     //
    S(94, 24),      S(91, 48),      S(105, 63),     S(107, 106),    S(110, 107),    S(74, 71),      S(51, 41),      S(37, 36),      //
    S(75, 17),      S(103, 6),      S(100, 50),     S(92, 82),      S(80, 83),      S(74, 65),      S(61, 13),      S(22, 32),      //
    S(15, -36),     S(42, -15),     S(47, 24),      S(60, 59),      S(55, 57),      S(28, 19),      S(13, -7),      S(-27, -31),    //
    S(38, -24),     S(39, -7),      S(24, -11),     S(31, 17),      S(29, 13),      S(1, -35),      S(-32, 0),      S(-40, -88),    //
    S(-46, -71),    S(4, -18),      S(24, -37),     S(42, -24),     S(22, -6),      S(-14, -43),    S(-22, -16),    S(-67, -124),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-127, 27),    S(-179, 111),   S(-423, 184),   S(-259, 87),    S(-293, 140),   S(-298, 149),   S(-209, 126),   S(-121, 68),    //
    S(-30, -12),    S(-75, 95),     S(-37, 59),     S(-68, 69),     S(-67, 75),     S(-34, 46),     S(-8, 40),      S(-41, 22),     //
    S(38, 27),      S(12, 69),      S(31, 75),      S(24, 67),      S(23, 48),      S(17, 52),      S(13, 49),      S(17, 18),      //
    S(15, 4),       S(53, 25),      S(62, 44),      S(75, 63),      S(101, 49),     S(48, 22),      S(51, -2),      S(3, 1),        //
    S(45, -43),     S(47, 7),       S(81, 16),      S(94, 30),      S(78, 46),      S(68, 27),      S(22, 13),      S(19, -30),     //
    S(61, -26),     S(85, -24),     S(98, 9),       S(67, 35),      S(63, 19),      S(60, 21),      S(65, -1),      S(20, -14),     //
    S(31, -76),     S(113, -45),    S(71, -12),     S(49, 5),       S(32, 6),       S(52, -37),     S(50, -46),     S(44, -51),     //
    S(50, -73),     S(21, -9),      S(26, -0),      S(49, -26),     S(31, -14),     S(38, 20),      S(45, -12),     S(42, -52),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(190, 91),     S(222, 107),    S(164, 140),    S(149, 120),    S(215, 69),     S(178, 94),     S(191, 99),     S(164, 104),    //
    S(95, 149),     S(150, 148),    S(209, 110),    S(154, 111),    S(196, 92),     S(157, 116),    S(106, 146),    S(100, 149),    //
    S(65, 142),     S(188, 87),     S(215, 68),     S(192, 50),     S(192, 70),     S(132, 110),    S(132, 120),    S(77, 161),     //
    S(41, 109),     S(106, 113),    S(135, 84),     S(109, 89),     S(140, 72),     S(99, 116),     S(91, 126),     S(32, 153),     //
    S(7, 53),       S(73, 59),      S(69, 73),      S(40, 82),      S(51, 86),      S(38, 114),     S(18, 113),     S(-5, 119),     //
    S(20, -24),     S(91, -0),      S(81, 22),      S(61, 30),      S(72, 38),      S(43, 66),      S(44, 49),      S(-2, 59),      //
    S(-65, -20),    S(66, -59),     S(71, -23),     S(59, 10),      S(64, 8),       S(48, 24),      S(36, 9),       S(5, 17),       //
    S(-4, -42),     S(12, -2),      S(83, -19),     S(93, -22),     S(93, -16),     S(70, 10),      S(67, -1),      S(48, 5),       //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(69, 178),     S(139, 155),    S(47, 298),     S(-9, 392),     S(34, 338),     S(63, 255),     S(58, 176),     S(2, 225),      //
    S(48, 208),     S(54, 260),     S(15, 336),     S(-124, 469),   S(-73, 431),    S(-22, 328),    S(25, 197),     S(17, 180),     //
    S(4, 262),      S(34, 294),     S(-8, 382),     S(-52, 423),    S(-28, 400),    S(21, 283),     S(58, 181),     S(46, 132),     //
    S(15, 179),     S(3, 275),      S(-28, 322),    S(-51, 403),    S(-37, 406),    S(2, 260),      S(41, 174),     S(24, 137),     //
    S(7, 170),      S(-1, 196),     S(-15, 247),    S(-40, 315),    S(-29, 337),    S(-5, 266),     S(1, 198),      S(30, 102),     //
    S(6, 65),       S(29, 85),      S(25, 158),     S(1, 180),      S(10, 190),     S(12, 216),     S(34, 145),     S(19, 115),     //
    S(-20, -76),    S(7, -72),      S(12, 1),       S(36, 41),      S(31, 82),      S(34, 59),      S(8, 92),       S(22, 69),      //
    S(-31, -55),    S(-15, -206),   S(16, -213),    S(30, -100),    S(38, -24),     S(37, -55),     S(34, -28),     S(5, 23),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(102, -577),   S(486, 1),      S(296, 176),    S(3, 153),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(343, -181),   S(375, 134),    S(207, 179),    S(20, 136),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(187, 42),     S(225, 159),    S(118, 190),    S(-39, 166),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(15, 10),      S(93, 114),     S(-18, 161),    S(-92, 177),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-116, -2),    S(-33, 78),     S(-90, 141),    S(-172, 188),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-54, -30),    S(12, 35),      S(-73, 116),    S(-124, 160),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(22, -81),     S(29, -5),      S(-33, 56),     S(-109, 112),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-19, -198),   S(4, -81),      S(-77, -17),    S(-74, -38),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-87, -436);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -5), S(10, 11), S(10, 5), S(5, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(5, 4), S(2, 2), S(3, -0), S(4, -2),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, -0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(3, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(18, -20), S(-6, 5), S(-2, -9), S(9, -16), S(13, -27), S(7, -54), S(-4, -46), }},
  {{ S(3, 2), S(-21, 0), S(-16, 8), S(-9, 17), S(-6, 7), S(-9, -12), S(-19, -24), }},
  {{ S(-4, -0), S(-12, -9), S(-14, 20), S(-9, 19), S(-8, 12), S(-9, -16), S(-26, -43), }},
  {{ S(7, 8), S(-9, 18), S(-6, 40), S(-1, 44), S(-1, 39), S(4, 16), S(8, -14), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(10, 26), S(-5, 4), S(-7, 12), S(-5, 29), S(2, 50),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 12), S(-45, -145), S(-8, -45), S(0, 5), S(-2, 5), S(-5, 11), S(-5, 10), }},
  {{ S(10, 1), S(-24, -132), S(-6, -32), S(-3, 2), S(-0, -2), S(-7, 5), S(2, 3), }},
  {{ S(0, 11), S(-10, -83), S(7, -15), S(1, 9), S(-3, 11), S(-9, 20), S(-5, 21), }},
  {{ S(2, 1), S(3, -63), S(-1, 29), S(-2, 26), S(-6, 10), S(-12, 13), S(-10, 21), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1362, 848, -22, -7
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(102);
inline VParam WINNABLE_ASYM = V(84);
inline VParam WINNABLE_PAWN_ENDGAME = V(105);
inline VParam WINNABLE_BIAS = V(-379);

// Epoch duration: 6.95597s
// clang-format on
}  // namespace Clockwork
