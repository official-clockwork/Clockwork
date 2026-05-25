#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(189, 491);
inline const PParam KNIGHT_MAT = S(801, 1533);
inline const PParam BISHOP_MAT = S(847, 1631);
inline const PParam ROOK_MAT   = S(1010, 2571);
inline const PParam QUEEN_MAT  = S(2384, 3882);
inline const PParam TEMPO_VAL  = S(65, 33);

inline const PParam BISHOP_XRAY_PAWNS   = S(-13, -7);
inline const PParam BISHOP_PAIR_VAL   = S(63, 223);
inline const PParam ROOK_OPEN_VAL     = S(108, -13);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, -2);
inline const PParam MINOR_BEHIND_PAWN = S(15, 36);
inline const PParam RESTRICTED_SQUARES = S(-24, -6);

inline const PParam DOUBLED_PAWN_VAL = S(-21, -76);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -38);

inline const PParam POTENTIAL_CHECKER_VAL = S(-46, -37);
inline const PParam OUTPOST_KNIGHT_VAL    = S(56, 45);
inline const PParam OUTPOST_BISHOP_VAL    = S(48, 40);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 52);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(39, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 52);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(58, -41);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(15, -4), S(30, 38), S(57, 68), S(126, 197), S(421, 287), S(582, 600),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(45, 34), S(38, 28), S(55, 64), S(99, 194), S(401, 137),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-91, -266), S(-89, -230), S(-67, -94), S(-25, 41), S(72, 232), S(211, 390),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(25, -43), S(25, -6), S(16, 33), S(15, 111), S(80, 238), S(148, 425),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -39), S(4, 7), S(-5, -23), S(-4, -55), S(-9, -165), S(-163, -326),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(16, 234), S(-0, 185), S(-2, 108), S(8, 68), S(19, 70), S(59, 67), S(47, 50),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-128, -152), S(15, 21), S(2, 113), S(27, 161), S(38, 197), S(51, 202), S(34, 184),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-82, -365), S(-22, -135), S(7, -19), S(31, 22), S(59, 48), S(73, 80), S(91, 84), S(110, 89), S(128, 42),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-59, -273), S(-12, -109), S(37, -39), S(59, 9), S(80, 46), S(92, 71), S(98, 92), S(102, 108), S(106, 119), S(112, 123), S(122, 114), S(137, 95), S(142, 98), S(121, 68),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(108, -203), S(24, -16), S(45, 18), S(61, 33), S(71, 52), S(75, 66), S(80, 80), S(85, 86), S(90, 98), S(96, 103), S(102, 108), S(105, 114), S(108, 117), S(112, 107), S(141, 58),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-210, -129), S(-61, -21), S(-35, 99), S(-13, 219), S(3, 262), S(10, 314), S(16, 348), S(22, 364), S(25, 389), S(27, 409), S(32, 416), S(35, 427), S(38, 430), S(39, 436), S(39, 437), S(34, 443), S(30, 443), S(31, 434), S(31, 427), S(32, 420), S(26, 406), S(28, 382), S(-3, 409), S(-46, 391), S(-80, 398), S(-116, 419), S(-159, 433), S(-132, 355),
};

inline const PParam PAWN_THREAT_KNIGHT = S(200, 147);
inline const PParam PAWN_THREAT_BISHOP = S(193, 215);
inline const PParam PAWN_THREAT_ROOK   = S(206, 155);
inline const PParam PAWN_THREAT_QUEEN  = S(175, 25);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(7, 69), S(96, 89), S(113, 126), S(215, 133), S(180, 30), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(4, 67), S(68, 83), S(79, 94), S(-19, 34), S(205, -18), S(0, 0),
};
inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(5, -19), S(4, -16), S(-0, -21), S(-6, -29), S(-11, -37), S(-16, -44), S(-18, -57), S(-24, -61), S(-27, -80),
};

inline const PParam ROOK_LINEUP = S(15, 75);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(183, 263),    S(38, 365),     S(70, 340),     S(135, 245),    S(192, 153),    S(120, 214),    S(84, 255),     S(222, 205),    //
    S(40, 49),      S(46, 91),      S(30, 33),      S(42, -40),     S(35, -74),     S(8, -34),      S(1, 23),       S(-22, 53),     //
    S(33, -18),     S(6, -8),       S(37, -53),     S(24, -74),     S(16, -94),     S(-6, -78),     S(-40, -44),    S(-42, -5),     //
    S(-9, -73),     S(-34, -41),    S(1, -64),      S(-9, -74),     S(-29, -85),    S(-39, -74),    S(-86, -54),    S(-78, -52),    //
    S(-13, -106),   S(20, -104),    S(5, -46),      S(-9, -52),     S(-35, -64),    S(-51, -73),    S(-79, -69),    S(-81, -73),    //
    S(9, -101),     S(81, -90),     S(69, -38),     S(23, -15),     S(-5, -40),     S(-24, -62),    S(-49, -57),    S(-61, -55),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-256, -523),  S(-254, 31),    S(-282, -99),   S(17, 1),       S(-84, 14),     S(-255, 53),    S(-379, 102),   S(-344, -396),  //
    S(-12, -29),    S(-9, 15),      S(97, -8),      S(80, 39),      S(93, 26),      S(52, 10),      S(-22, 16),     S(-32, -5),     //
    S(39, -42),     S(37, 24),      S(75, 47),      S(82, 56),      S(69, 59),      S(25, 64),      S(15, 22),      S(-36, -3),     //
    S(98, 7),       S(92, 37),      S(105, 65),     S(107, 104),    S(114, 105),    S(79, 75),      S(53, 36),      S(40, 21),      //
    S(75, 10),      S(106, 3),      S(103, 57),     S(94, 87),      S(81, 88),      S(80, 71),      S(66, 12),      S(24, 21),      //
    S(18, -39),     S(47, -13),     S(50, 33),      S(64, 65),      S(59, 63),      S(32, 29),      S(19, -6),      S(-23, -43),    //
    S(38, -29),     S(45, -18),     S(27, -9),      S(34, 19),      S(31, 14),      S(6, -37),      S(-27, -5),     S(-33, -114),   //
    S(-40, -91),    S(9, -30),      S(30, -44),     S(48, -34),     S(28, -11),     S(-8, -49),     S(-18, -30),    S(-59, -146),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-121, 15),    S(-172, 99),    S(-456, 192),   S(-277, 86),    S(-294, 130),   S(-299, 141),   S(-199, 110),   S(-118, 47),    //
    S(-33, -26),    S(-79, 91),     S(-36, 47),     S(-76, 70),     S(-65, 64),     S(-33, 41),     S(-8, 32),      S(-47, 8),      //
    S(36, 17),      S(14, 67),      S(35, 70),      S(33, 60),      S(26, 41),      S(21, 47),      S(13, 49),      S(18, 7),       //
    S(15, -11),     S(57, 17),      S(68, 38),      S(79, 59),      S(107, 42),     S(50, 27),      S(53, -5),      S(6, -13),      //
    S(51, -61),     S(51, -4),      S(83, 12),      S(101, 31),     S(84, 48),      S(72, 23),      S(26, 7),       S(22, -40),     //
    S(64, -43),     S(87, -28),     S(101, 8),      S(71, 38),      S(69, 20),      S(66, 21),      S(74, -10),     S(24, -27),     //
    S(35, -98),     S(117, -54),    S(75, -12),     S(53, 2),       S(36, 5),       S(55, -41),     S(53, -52),     S(48, -73),     //
    S(52, -89),     S(22, -17),     S(26, -4),      S(49, -29),     S(33, -23),     S(40, 13),      S(50, -22),     S(45, -63),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(162, 87),     S(202, 97),     S(153, 123),    S(141, 99),     S(198, 61),     S(157, 84),     S(174, 91),     S(144, 102),    //
    S(82, 135),     S(147, 123),    S(196, 93),     S(147, 94),     S(192, 75),     S(142, 105),    S(95, 131),     S(89, 135),     //
    S(57, 135),     S(180, 80),     S(208, 60),     S(187, 39),     S(183, 63),     S(127, 102),    S(130, 108),    S(72, 153),     //
    S(36, 102),     S(101, 110),    S(126, 81),     S(106, 84),     S(134, 72),     S(96, 111),     S(93, 119),     S(30, 146),     //
    S(8, 43),       S(78, 51),      S(66, 68),      S(41, 75),      S(50, 84),      S(38, 110),     S(17, 109),     S(-3, 107),     //
    S(21, -30),     S(92, -9),      S(80, 16),      S(60, 22),      S(71, 28),      S(39, 63),      S(47, 39),      S(1, 50),       //
    S(-67, -26),    S(68, -69),     S(70, -29),     S(57, 7),       S(61, 4),       S(46, 18),      S(36, 0),       S(8, 6),        //
    S(-4, -60),     S(8, -16),      S(80, -36),     S(90, -35),     S(89, -27),     S(68, -3),      S(67, -17),     S(49, -11),     //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(53, 162),     S(142, 109),    S(42, 262),     S(-16, 365),    S(24, 320),     S(60, 224),     S(58, 147),     S(-8, 200),     //
    S(48, 173),     S(59, 245),     S(22, 323),     S(-119, 458),   S(-61, 419),    S(-12, 320),    S(38, 173),     S(12, 163),     //
    S(-2, 237),     S(30, 286),     S(-3, 369),     S(-44, 411),    S(-23, 395),    S(27, 280),     S(60, 167),     S(51, 100),     //
    S(15, 144),     S(4, 255),      S(-25, 319),    S(-46, 399),    S(-27, 401),    S(8, 262),      S(49, 159),     S(24, 121),     //
    S(5, 137),      S(-3, 177),     S(-17, 242),    S(-37, 312),    S(-23, 337),    S(-2, 259),     S(9, 170),      S(33, 78),      //
    S(5, 35),       S(27, 62),      S(21, 151),     S(-3, 174),     S(9, 176),      S(11, 207),     S(33, 127),     S(22, 85),      //
    S(-25, -117),   S(3, -95),      S(8, -13),      S(31, 23),      S(26, 65),      S(29, 40),      S(4, 67),       S(19, 43),      //
    S(-38, -97),    S(-23, -241),   S(7, -241),     S(22, -129),    S(30, -51),     S(31, -83),     S(29, -58),     S(2, -16),      //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(134, -595),   S(481, 5),      S(293, 166),    S(16, 131),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(340, -171),   S(351, 142),    S(185, 197),    S(8, 157),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(170, 69),     S(195, 204),    S(96, 224),     S(-41, 182),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-3, 27),      S(67, 154),     S(-41, 192),    S(-95, 199),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-118, 4),     S(-40, 92),     S(-104, 156),   S(-176, 196),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-48, -34),    S(11, 40),      S(-71, 117),    S(-121, 158),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(23, -90),     S(29, -9),      S(-33, 54),     S(-106, 105),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-19, -203),   S(4, -87),      S(-76, -25),    S(-71, -47),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const PParam KS_NO_QUEEN = S(-97, -437);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -7), S(10, 9), S(10, 2), S(4, 0), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 3), S(2, 2), S(3, -0), S(3, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-4, 0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -1);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(18, -19), S(-6, 5), S(-1, -9), S(10, -17), S(15, -30), S(9, -54), S(-2, -45), }},
  {{ S(2, 2), S(-21, 2), S(-16, 8), S(-9, 16), S(-6, 7), S(-7, -20), S(-18, -24), }},
  {{ S(-3, -2), S(-11, -9), S(-13, 17), S(-7, 16), S(-7, 9), S(-7, -20), S(-22, -39), }},
  {{ S(8, 7), S(-9, 15), S(-5, 35), S(-1, 41), S(-1, 37), S(6, 13), S(11, -18), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(9, 27), S(-4, 4), S(-7, 11), S(-5, 28), S(2, 48),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(6, 11), S(-45, -157), S(-9, -53), S(0, 5), S(-1, 4), S(-5, 11), S(-4, 9), }},
  {{ S(10, -1), S(-26, -174), S(-5, -35), S(-2, 1), S(1, -3), S(-6, 4), S(3, 2), }},
  {{ S(1, 10), S(-10, -96), S(8, -21), S(1, 9), S(-2, 11), S(-9, 19), S(-5, 20), }},
  {{ S(2, 1), S(5, -70), S(-1, 28), S(-2, 26), S(-5, 10), S(-11, 12), S(-9, 21), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1400, 893, -19, -7
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(83);
inline VParam WINNABLE_PAWN_ENDGAME = V(102);
inline VParam WINNABLE_BIAS = V(-368);

// Epoch duration: 7.9106s
// clang-format on
}  // namespace Clockwork
