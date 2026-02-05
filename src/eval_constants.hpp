#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(136, 203);
inline const PParam KNIGHT_MAT = S(575, 624);
inline const PParam BISHOP_MAT = S(605, 648);
inline const PParam ROOK_MAT   = S(474, 682);
inline const PParam QUEEN_MAT  = S(1095, 1087);
inline const PParam TEMPO_VAL  = S(57, 21);

inline const PParam BISHOP_PAIR_VAL   = S(52, 196);
inline const PParam ROOK_OPEN_VAL     = S(105, -11);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 22);

inline const PParam DOUBLED_PAWN_VAL = S(-7, -70);
inline const PParam ISOLATED_PAWN_VAL = S(-19, -17);

inline const PParam POTENTIAL_CHECKER_VAL = S(-49, -24);
inline const PParam OUTPOST_KNIGHT_VAL    = S(36, 64);
inline const PParam OUTPOST_BISHOP_VAL    = S(50, 39);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(42, 7);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(54, -19);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(27, 53);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(67, -36);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(16, 1), S(38, 27), S(57, 51), S(136, 145), S(383, 197), S(456, 694),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(54, 35), S(44, 27), S(53, 55), S(118, 140), S(411, 79),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-70, -121), S(-67, -98), S(-47, 14), S(9, 122), S(91, 268), S(261, 382),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(36, -45), S(35, -5), S(23, 31), S(1, 107), S(46, 219), S(200, 280),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(23, -38), S(2, 13), S(3, -21), S(1, -56), S(-6, -128), S(-204, -238),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-9, 151), S(-18, 113), S(-11, 43), S(-3, 6), S(2, 5), S(37, 5), S(10, -1),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-314, -47), S(-47, 30), S(-27, 61), S(12, 96), S(31, 112), S(49, 112), S(30, 105),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(56, 0), S(127, 171), S(162, 261), S(190, 297), S(230, 315), S(254, 350), S(286, 347), S(317, 361), S(371, 274),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(90, 2), S(133, 175), S(187, 239), S(214, 284), S(239, 315), S(255, 339), S(262, 359), S(276, 369), S(282, 386), S(307, 368), S(317, 371), S(366, 317), S(364, 332), S(394, 279),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(307, 241), S(226, 420), S(251, 445), S(268, 455), S(279, 467), S(286, 478), S(292, 488), S(301, 490), S(307, 501), S(318, 502), S(330, 503), S(337, 508), S(342, 507), S(359, 479), S(446, 374),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(493, 512), S(633, 531), S(658, 643), S(676, 754), S(692, 801), S(700, 845), S(706, 877), S(714, 887), S(717, 911), S(721, 925), S(726, 932), S(731, 940), S(740, 933), S(742, 938), S(747, 934), S(745, 938), S(747, 933), S(754, 925), S(761, 916), S(770, 910), S(778, 887), S(801, 847), S(800, 844), S(797, 791), S(786, 777), S(777, 749), S(709, 810), S(733, 714),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(496, -200), S(107, -106), S(29, -28), S(16, 6), S(-3, 5), S(-20, 1), S(-25, 8), S(-41, 16), S(-31, -22),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(12, -8), S(16, -15), S(13, -12), S(6, -8), S(2, 205),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -0), S(7, -11), S(6, -9), S(5, -5), S(6, 414),
};

inline const PParam PAWN_THREAT_KNIGHT = S(212, 92);
inline const PParam PAWN_THREAT_BISHOP = S(190, 143);
inline const PParam PAWN_THREAT_ROOK   = S(192, 123);
inline const PParam PAWN_THREAT_QUEEN  = S(167, -17);

inline const PParam KNIGHT_THREAT_BISHOP = S(109, 101);
inline const PParam KNIGHT_THREAT_ROOK   = S(225, 68);
inline const PParam KNIGHT_THREAT_QUEEN  = S(150, -2);

inline const PParam BISHOP_THREAT_KNIGHT = S(105, 57);
inline const PParam BISHOP_THREAT_ROOK   = S(212, 124);
inline const PParam BISHOP_THREAT_QUEEN  = S(175, 98);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(9, -35), S(-4, -9), S(-6, -18), S(-9, -28), S(-13, -37), S(-18, -42), S(-20, -55), S(-27, -52), S(-38, -53),
};

inline const PParam ROOK_LINEUP = S(12, 66);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(294, 346),    S(145, 444),    S(290, 413),    S(303, 278),    S(320, 238),    S(214, 322),    S(151, 359),    S(258, 315),    //
    S(114, 212),    S(145, 252),    S(186, 167),    S(172, 103),    S(152, 86),     S(125, 139),    S(95, 197),     S(47, 222),     //
    S(80, 174),     S(67, 176),     S(110, 101),    S(111, 79),     S(100, 76),     S(71, 109),     S(23, 159),     S(10, 188),     //
    S(57, 125),     S(46, 149),     S(82, 96),      S(79, 88),      S(58, 91),      S(40, 117),     S(-20, 160),    S(-20, 152),    //
    S(56, 98),      S(95, 105),     S(80, 137),     S(72, 122),     S(44, 118),     S(21, 125),     S(-13, 142),    S(-21, 135),    //
    S(69, 100),     S(158, 116),    S(143, 172),    S(94, 154),     S(66, 141),     S(45, 136),     S(15, 148),     S(-3, 150),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-251, -76),   S(-156, 196),   S(-102, 47),    S(94, 182),     S(32, 184),     S(-115, 189),   S(-246, 237),   S(-291, -3),    //
    S(75, 159),     S(116, 187),    S(204, 145),    S(197, 156),    S(194, 162),    S(111, 187),    S(74, 203),     S(48, 180),     //
    S(159, 156),    S(206, 152),    S(207, 209),    S(208, 193),    S(167, 229),    S(107, 244),    S(119, 189),    S(84, 182),     //
    S(227, 178),    S(235, 198),    S(231, 223),    S(218, 254),    S(219, 262),    S(173, 248),    S(166, 216),    S(161, 188),    //
    S(210, 183),    S(245, 165),    S(214, 223),    S(211, 242),    S(187, 247),    S(185, 231),    S(182, 181),    S(145, 188),    //
    S(155, 149),    S(180, 159),    S(170, 197),    S(178, 224),    S(174, 223),    S(138, 203),    S(128, 171),    S(100, 136),    //
    S(166, 152),    S(187, 150),    S(155, 159),    S(156, 183),    S(148, 180),    S(121, 137),    S(108, 157),    S(95, 76),      //
    S(101, 104),    S(147, 169),    S(162, 136),    S(179, 128),    S(159, 152),    S(127, 124),    S(112, 148),    S(75, 71),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-2, 293),     S(-17, 304),    S(-279, 348),   S(-144, 284),   S(-155, 306),   S(-169, 304),   S(-52, 300),    S(30, 273),     //
    S(121, 202),    S(61, 285),     S(94, 240),     S(40, 259),     S(56, 257),     S(81, 257),     S(119, 252),    S(95, 231),     //
    S(191, 235),    S(204, 246),    S(196, 256),    S(173, 255),    S(144, 251),    S(152, 259),    S(167, 251),    S(164, 222),    //
    S(161, 224),    S(197, 235),    S(198, 255),    S(194, 294),    S(224, 282),    S(155, 259),    S(168, 223),    S(125, 225),    //
    S(189, 184),    S(206, 216),    S(211, 240),    S(215, 268),    S(191, 283),    S(177, 260),    S(144, 238),    S(141, 195),    //
    S(216, 191),    S(249, 204),    S(260, 231),    S(205, 257),    S(202, 235),    S(196, 247),    S(203, 224),    S(144, 219),    //
    S(182, 165),    S(281, 182),    S(238, 196),    S(205, 213),    S(185, 217),    S(189, 193),    S(190, 190),    S(183, 184),    //
    S(213, 167),    S(201, 184),    S(200, 217),    S(214, 180),    S(204, 191),    S(204, 233),    S(200, 212),    S(192, 189),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(425, 384),    S(437, 403),    S(413, 417),    S(433, 378),    S(413, 384),    S(378, 396),    S(374, 412),    S(369, 415),    //
    S(303, 465),    S(378, 462),    S(453, 429),    S(400, 433),    S(385, 441),    S(364, 446),    S(297, 477),    S(280, 483),    //
    S(272, 460),    S(373, 448),    S(430, 408),    S(411, 395),    S(362, 424),    S(320, 447),    S(323, 444),    S(257, 484),    //
    S(276, 420),    S(353, 429),    S(389, 402),    S(363, 406),    S(354, 417),    S(315, 439),    S(311, 434),    S(236, 461),    //
    S(259, 352),    S(323, 370),    S(312, 390),    S(291, 387),    S(268, 411),    S(258, 424),    S(228, 418),    S(208, 415),    //
    S(251, 296),    S(313, 323),    S(299, 349),    S(284, 347),    S(271, 356),    S(245, 379),    S(243, 353),    S(208, 361),    //
    S(144, 307),    S(276, 273),    S(285, 304),    S(275, 326),    S(260, 331),    S(253, 334),    S(235, 321),    S(211, 331),    //
    S(208, 293),    S(231, 313),    S(287, 303),    S(299, 300),    S(288, 308),    S(268, 323),    S(257, 320),    S(238, 328),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(553, 638),    S(603, 589),    S(528, 712),    S(493, 783),    S(478, 768),    S(515, 706),    S(534, 633),    S(489, 669),    //
    S(553, 691),    S(515, 789),    S(503, 825),    S(358, 922),    S(365, 911),    S(430, 855),    S(475, 729),    S(485, 709),    //
    S(514, 762),    S(561, 782),    S(494, 885),    S(443, 903),    S(417, 900),    S(468, 818),    S(504, 738),    S(525, 657),    //
    S(554, 680),    S(558, 767),    S(493, 841),    S(486, 869),    S(477, 867),    S(489, 773),    S(531, 707),    S(518, 672),    //
    S(568, 649),    S(555, 721),    S(525, 778),    S(496, 818),    S(494, 820),    S(497, 766),    S(514, 690),    S(542, 610),    //
    S(548, 593),    S(582, 643),    S(573, 712),    S(543, 705),    S(539, 693),    S(534, 720),    S(549, 637),    S(528, 609),    //
    S(529, 480),    S(567, 493),    S(570, 561),    S(582, 599),    S(569, 626),    S(563, 590),    S(525, 630),    S(537, 605),    //
    S(511, 496),    S(544, 343),    S(567, 363),    S(581, 474),    S(577, 549),    S(575, 496),    S(564, 520),    S(532, 549),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-69, -319),   S(265, 196),    S(140, 151),    S(25, -28),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(212, -52),    S(327, 175),    S(185, 165),    S(116, 3),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(202, 46),     S(357, 119),    S(250, 117),    S(159, 6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(54, 16),      S(304, 59),     S(158, 76),     S(105, 33),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-33, -18),    S(219, 14),     S(151, 20),     S(27, 32),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-23, -15),    S(212, -3),     S(114, 8),      S(64, -11),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(1, -21),      S(85, 32),      S(2, 34),       S(-55, 10),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-112, -72),   S(-13, -26),    S(-113, -20),   S(-116, -89),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(20, -17), S(2, 19), S(-2, 21), S(5, 7), S(5, 4), S(-2, 21), S(4, 20), }},
  {{ S(10, -5), S(-18, 28), S(-14, 22), S(-5, 4), S(-4, 4), S(-13, 26), S(-17, 36), }},
  {{ S(-12, 3), S(-23, 28), S(-22, 10), S(-18, -5), S(-18, -3), S(-21, 10), S(-22, 25), }},
  {{ S(6, -15), S(-12, -15), S(-3, -19), S(2, -23), S(0, -21), S(-4, -16), S(-8, -16), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(4, 23), S(-18, 31), S(-15, 19), S(-8, 7), S(-8, -8),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-4, 2), S(-15, 10), S(-13, 9), S(-12, 9), S(-13, 3), S(-11, -1), S(-15, 8), }},
  {{ S(-3, 1), S(-11, 7), S(-18, 12), S(-15, 9), S(-14, 5), S(-16, 9), S(-11, 6), }},
  {{ S(-10, 5), S(-14, 2), S(-15, 9), S(-11, 8), S(-11, 7), S(-15, 7), S(-14, 3), }},
  {{ S(-10, 7), S(-15, 9), S(-16, 8), S(-11, 12), S(-9, 12), S(-15, 8), S(-17, 8), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1166, 539, -40, 58
);

inline VParam WINNABLE_PAWNS = V(-15);
inline VParam WINNABLE_SYM = V(135);
inline VParam WINNABLE_ASYM = V(123);
inline VParam WINNABLE_BIAS = V(-642);

// Epoch duration: 7.52381s
// clang-format on
}  // namespace Clockwork
