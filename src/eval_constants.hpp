#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(189, 484);
inline const PParam KNIGHT_MAT = S(804, 1521);
inline const PParam BISHOP_MAT = S(842, 1618);
inline const PParam ROOK_MAT   = S(1013, 2561);
inline const PParam QUEEN_MAT  = S(2374, 3864);
inline const PParam TEMPO_VAL  = S(61, 26);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, 13);
inline const PParam BISHOP_PAIR_VAL   = S(63, 228);
inline const PParam ROOK_OPEN_VAL     = S(118, -10);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 37);
inline const PParam MINOR_BEHIND_PAWN = S(17, 39);
inline const PParam RESTRICTED_SQUARES = S(-23, -7);

inline const PParam DOUBLED_PAWN_VAL = S(-21, -75);
inline const PParam ISOLATED_PAWN_VAL = S(-14, -39);

inline const PParam POTENTIAL_CHECKER_VAL = S(-44, -40);
inline const PParam OUTPOST_KNIGHT_VAL    = S(57, 55);
inline const PParam OUTPOST_BISHOP_VAL    = S(55, 45);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(36, 10);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(39, -2);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(24, 60);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(58, -41);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -2), S(28, 37), S(58, 68), S(127, 200), S(424, 291), S(587, 605),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(45, 45), S(41, 41), S(60, 77), S(103, 209), S(406, 169),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-90, -266), S(-87, -229), S(-66, -92), S(-24, 41), S(71, 231), S(211, 380),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(24, -41), S(25, -8), S(16, 32), S(16, 109), S(84, 232), S(146, 430),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(18, -37), S(5, 11), S(-4, -23), S(-0, -55), S(-7, -167), S(-170, -331),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(17, 229), S(-1, 183), S(-3, 105), S(6, 64), S(18, 65), S(58, 63), S(48, 46),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-125, -149), S(14, 22), S(2, 114), S(27, 161), S(38, 197), S(51, 203), S(36, 183),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-87, -365), S(-27, -147), S(2, -30), S(25, 15), S(54, 42), S(69, 80), S(87, 86), S(105, 95), S(124, 45),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-60, -290), S(-13, -126), S(35, -54), S(57, 0), S(79, 41), S(91, 70), S(97, 93), S(102, 110), S(105, 121), S(109, 125), S(118, 113), S(131, 92), S(133, 93), S(111, 60),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(109, -191), S(26, -12), S(47, 21), S(63, 35), S(73, 53), S(77, 67), S(83, 81), S(89, 85), S(94, 97), S(102, 101), S(109, 105), S(112, 111), S(111, 115), S(112, 99), S(137, 46),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-212, -131), S(-66, -25), S(-39, 93), S(-18, 216), S(-3, 258), S(4, 310), S(11, 342), S(16, 359), S(20, 383), S(23, 401), S(27, 409), S(30, 420), S(32, 425), S(32, 432), S(32, 434), S(26, 441), S(21, 442), S(23, 431), S(22, 427), S(25, 418), S(20, 404), S(22, 380), S(-8, 405), S(-51, 389), S(-84, 395), S(-121, 416), S(-163, 429), S(-136, 353),
};

inline const PParam PAWN_THREAT_KNIGHT = S(199, 143);
inline const PParam PAWN_THREAT_BISHOP = S(185, 194);
inline const PParam PAWN_THREAT_ROOK   = S(202, 149);
inline const PParam PAWN_THREAT_QUEEN  = S(169, 25);

inline const PParam KNIGHT_THREAT_BISHOP = S(109, 126);
inline const PParam KNIGHT_THREAT_ROOK   = S(223, 97);
inline const PParam KNIGHT_THREAT_QUEEN  = S(163, -25);

inline const PParam BISHOP_THREAT_KNIGHT = S(97, 80);
inline const PParam BISHOP_THREAT_ROOK   = S(208, 163);
inline const PParam BISHOP_THREAT_QUEEN  = S(181, 91);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -19), S(3, -17), S(-1, -23), S(-6, -32), S(-11, -40), S(-16, -46), S(-18, -59), S(-23, -61), S(-27, -76),
};

inline const PParam ROOK_LINEUP = S(17, 74);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(184, 255),    S(37, 359),     S(65, 337),     S(132, 241),    S(190, 144),    S(119, 208),    S(83, 250),     S(223, 199),    //
    S(35, 46),      S(43, 84),      S(25, 26),      S(39, -45),     S(33, -81),     S(3, -40),      S(-3, 16),      S(-26, 47),     //
    S(27, -22),     S(2, -13),      S(32, -57),     S(19, -80),     S(9, -99),      S(-10, -84),    S(-44, -49),    S(-46, -10),    //
    S(-13, -77),    S(-36, -44),    S(-2, -66),     S(-12, -78),    S(-32, -89),    S(-41, -78),    S(-88, -57),    S(-81, -57),    //
    S(-15, -108),   S(19, -107),    S(7, -46),      S(-9, -53),     S(-36, -65),    S(-50, -73),    S(-79, -71),    S(-82, -76),    //
    S(8, -102),     S(83, -91),     S(71, -40),     S(24, -15),     S(-4, -39),     S(-24, -60),    S(-48, -56),    S(-61, -56),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-265, -515),  S(-284, 60),    S(-305, -79),   S(-9, 25),      S(-113, 42),    S(-283, 74),    S(-409, 124),   S(-358, -382),  //
    S(-28, -23),    S(-20, 27),     S(78, 0),       S(62, 46),      S(76, 30),      S(36, 14),      S(-31, 21),     S(-47, 1),      //
    S(33, -26),     S(30, 22),      S(73, 55),      S(80, 72),      S(70, 77),      S(24, 70),      S(9, 17),       S(-44, 12),     //
    S(93, 18),      S(96, 51),      S(106, 69),     S(109, 109),    S(115, 104),    S(80, 75),      S(56, 43),      S(35, 29),      //
    S(71, 12),      S(105, 3),      S(100, 54),     S(90, 78),      S(78, 76),      S(78, 65),      S(61, 11),      S(20, 22),      //
    S(15, -44),     S(45, -14),     S(49, 22),      S(62, 53),      S(56, 49),      S(30, 17),      S(18, -10),     S(-26, -48),    //
    S(35, -32),     S(41, -25),     S(23, -23),     S(31, -0),      S(27, -3),      S(2, -51),      S(-32, -8),     S(-38, -116),   //
    S(-43, -89),    S(8, -36),      S(29, -58),     S(48, -46),     S(28, -26),     S(-8, -61),     S(-19, -36),    S(-62, -143),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-125, 12),    S(-186, 115),   S(-481, 214),   S(-301, 103),   S(-325, 150),   S(-328, 159),   S(-216, 128),   S(-126, 49),    //
    S(-46, -23),    S(-98, 94),     S(-53, 49),     S(-103, 75),    S(-89, 68),     S(-48, 41),     S(-26, 35),     S(-58, 15),     //
    S(30, 15),      S(8, 67),       S(23, 69),      S(19, 61),      S(12, 42),      S(10, 45),      S(6, 50),       S(12, 4),       //
    S(8, -3),       S(51, 14),      S(62, 41),      S(76, 65),      S(105, 51),     S(46, 26),      S(48, -10),     S(-1, -11),     //
    S(50, -61),     S(48, -4),      S(81, 11),      S(96, 30),      S(81, 45),      S(72, 22),      S(22, 3),       S(19, -39),     //
    S(63, -39),     S(88, -27),     S(102, 3),      S(71, 28),      S(67, 7),       S(66, 14),      S(75, -10),     S(22, -24),     //
    S(35, -93),     S(117, -66),    S(75, -30),     S(53, -14),     S(36, -11),     S(55, -52),     S(53, -61),     S(48, -69),     //
    S(54, -89),     S(25, -26),     S(29, -13),     S(54, -39),     S(37, -33),     S(43, 4),       S(52, -30),     S(47, -66),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(199, 83),     S(229, 95),     S(172, 123),    S(164, 95),     S(228, 53),     S(185, 79),     S(210, 84),     S(188, 94),     //
    S(95, 161),     S(166, 149),    S(232, 113),    S(173, 127),    S(218, 112),    S(176, 135),    S(116, 160),    S(105, 163),    //
    S(61, 153),     S(198, 94),     S(218, 83),     S(205, 58),     S(200, 84),     S(138, 126),    S(140, 127),    S(73, 177),     //
    S(32, 117),     S(107, 128),    S(134, 99),     S(112, 100),    S(142, 87),     S(101, 130),    S(94, 138),     S(26, 160),     //
    S(1, 44),       S(78, 53),      S(64, 73),      S(39, 71),      S(49, 81),      S(37, 109),     S(13, 109),     S(-8, 107),     //
    S(10, -39),     S(88, -19),     S(75, 5),       S(53, 8),       S(65, 11),      S(36, 47),      S(41, 27),      S(-8, 40),      //
    S(-77, -41),    S(60, -86),     S(59, -48),     S(47, -14),     S(53, -16),     S(37, -3),      S(27, -18),     S(-2, -11),     //
    S(-16, -74),    S(-2, -33),     S(68, -51),     S(76, -50),     S(77, -41),     S(56, -18),     S(56, -32),     S(37, -25),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(27, 181),     S(126, 120),    S(31, 265),     S(-35, 375),    S(7, 328),      S(41, 236),     S(42, 154),     S(-30, 215),    //
    S(40, 179),     S(48, 252),     S(10, 328),     S(-133, 460),   S(-77, 426),    S(-25, 322),    S(25, 180),     S(7, 166),      //
    S(-6, 239),     S(23, 289),     S(-11, 369),    S(-54, 412),    S(-32, 397),    S(17, 285),     S(53, 172),     S(48, 101),     //
    S(12, 144),     S(1, 253),      S(-32, 316),    S(-54, 397),    S(-35, 399),    S(2, 260),      S(46, 156),     S(23, 119),     //
    S(5, 128),      S(-5, 171),     S(-21, 236),    S(-42, 304),    S(-27, 328),    S(-3, 250),     S(7, 167),      S(33, 74),      //
    S(5, 28),       S(28, 50),      S(21, 137),     S(-2, 161),     S(8, 168),      S(12, 198),     S(34, 121),     S(24, 76),      //
    S(-23, -130),   S(6, -112),     S(8, -28),      S(31, 10),      S(25, 56),      S(30, 29),      S(4, 61),       S(20, 37),      //
    S(-34, -107),   S(-18, -256),   S(12, -257),    S(28, -146),    S(34, -64),     S(36, -97),     S(34, -67),     S(7, -25),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(131, -581),   S(470, 19),     S(288, 174),    S(19, 135),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(331, -160),   S(346, 146),    S(180, 201),    S(12, 154),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(167, 70),     S(187, 207),    S(93, 225),     S(-43, 183),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-4, 27),      S(61, 156),     S(-44, 191),    S(-97, 199),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-121, 2),     S(-44, 91),     S(-108, 155),   S(-182, 197),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-50, -35),    S(8, 38),       S(-75, 115),    S(-122, 155),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(24, -92),     S(28, -11),     S(-34, 50),     S(-104, 100),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-19, -199),   S(4, -85),      S(-76, -25),    S(-69, -49),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-97, -440);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -7), S(9, 10), S(10, 4), S(4, 0), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 3), S(2, 2), S(3, -0), S(3, -2),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-4, 0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(18, -20), S(-6, 4), S(-2, -9), S(9, -17), S(14, -30), S(8, -55), S(-2, -48), }},
  {{ S(1, 2), S(-22, 3), S(-17, 10), S(-10, 16), S(-7, 7), S(-8, -19), S(-19, -26), }},
  {{ S(-2, -1), S(-9, -8), S(-11, 19), S(-6, 18), S(-6, 10), S(-6, -18), S(-22, -39), }},
  {{ S(7, 6), S(-9, 15), S(-6, 35), S(-2, 40), S(-2, 35), S(6, 9), S(10, -20), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(8, 27), S(-4, 3), S(-7, 10), S(-5, 27), S(2, 48),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 10), S(-44, -146), S(-9, -52), S(0, 4), S(-1, 3), S(-5, 10), S(-4, 8), }},
  {{ S(10, -2), S(-25, -164), S(-5, -35), S(-2, 0), S(1, -4), S(-6, 3), S(3, 1), }},
  {{ S(1, 10), S(-10, -89), S(8, -21), S(0, 9), S(-3, 10), S(-9, 18), S(-5, 19), }},
  {{ S(2, 1), S(4, -64), S(-2, 30), S(-3, 26), S(-5, 10), S(-11, 12), S(-9, 20), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1412, 886, -16, -6
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(82);
inline VParam WINNABLE_PAWN_ENDGAME = V(104);
inline VParam WINNABLE_BIAS = V(-364);

// Epoch duration: 7.43877s
// clang-format on
}  // namespace Clockwork
