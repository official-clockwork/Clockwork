#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(136, 201);
inline const PParam KNIGHT_MAT = S(569, 624);
inline const PParam BISHOP_MAT = S(603, 648);
inline const PParam ROOK_MAT   = S(469, 678);
inline const PParam QUEEN_MAT  = S(1101, 1088);
inline const PParam TEMPO_VAL  = S(58, 21);

inline const PParam BISHOP_PAIR_VAL   = S(52, 197);
inline const PParam ROOK_OPEN_VAL     = S(105, -11);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 21);

inline const PParam DOUBLED_PAWN_VAL = S(-7, -70);
inline const PParam ISOLATED_PAWN_VAL = S(-19, -17);

inline const PParam POTENTIAL_CHECKER_VAL = S(-49, -25);
inline const PParam OUTPOST_KNIGHT_VAL    = S(35, 66);
inline const PParam OUTPOST_BISHOP_VAL    = S(49, 40);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(42, 7);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(54, -18);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(26, 55);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(67, -36);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(16, 1), S(38, 27), S(57, 51), S(135, 146), S(377, 206), S(459, 690),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(54, 35), S(44, 27), S(54, 54), S(117, 142), S(403, 90),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-71, -121), S(-67, -99), S(-46, 13), S(11, 119), S(92, 268), S(264, 380),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(36, -45), S(34, -4), S(22, 31), S(1, 109), S(47, 221), S(200, 283),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(22, -37), S(3, 12), S(3, -21), S(-1, -53), S(-8, -126), S(-211, -232),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-5, 149), S(-16, 113), S(-12, 45), S(-5, 9), S(0, 8), S(36, 6), S(10, 1),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-306, -54), S(-42, 26), S(-26, 61), S(11, 97), S(30, 114), S(48, 114), S(29, 107),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(66, 3), S(137, 174), S(172, 264), S(200, 300), S(240, 318), S(264, 353), S(296, 350), S(327, 364), S(380, 278),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(89, 0), S(132, 174), S(186, 238), S(213, 282), S(238, 314), S(254, 339), S(261, 358), S(275, 368), S(281, 386), S(306, 367), S(316, 369), S(364, 317), S(362, 332), S(393, 278),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(312, 247), S(231, 426), S(256, 451), S(273, 460), S(284, 473), S(291, 484), S(297, 494), S(306, 496), S(312, 507), S(323, 508), S(334, 509), S(342, 514), S(347, 513), S(364, 485), S(449, 382),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(494, 513), S(635, 531), S(659, 644), S(677, 756), S(693, 802), S(702, 847), S(708, 878), S(715, 889), S(719, 912), S(722, 927), S(728, 934), S(732, 942), S(741, 935), S(743, 941), S(748, 935), S(746, 940), S(749, 934), S(755, 928), S(762, 920), S(772, 912), S(780, 889), S(802, 849), S(800, 848), S(797, 794), S(785, 781), S(780, 749), S(712, 808), S(735, 713),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(497, -203), S(104, -103), S(26, -24), S(13, 9), S(-6, 9), S(-22, 1), S(-27, 8), S(-41, 14), S(-28, -28),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(12, -7), S(16, -15), S(13, -12), S(6, -8), S(2, 205),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -0), S(7, -11), S(6, -9), S(5, -5), S(6, 416),
};

inline const PParam PAWN_THREAT_KNIGHT = S(213, 93);
inline const PParam PAWN_THREAT_BISHOP = S(190, 143);
inline const PParam PAWN_THREAT_ROOK   = S(191, 124);
inline const PParam PAWN_THREAT_QUEEN  = S(166, -16);

inline const PParam KNIGHT_THREAT_BISHOP = S(109, 101);
inline const PParam KNIGHT_THREAT_ROOK   = S(227, 67);
inline const PParam KNIGHT_THREAT_QUEEN  = S(151, -2);

inline const PParam BISHOP_THREAT_KNIGHT = S(105, 57);
inline const PParam BISHOP_THREAT_ROOK   = S(212, 124);
inline const PParam BISHOP_THREAT_QUEEN  = S(175, 100);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(9, -36), S(-4, -9), S(-6, -18), S(-9, -28), S(-13, -37), S(-18, -42), S(-21, -54), S(-28, -52), S(-39, -52),
};

inline const PParam ROOK_LINEUP = S(12, 67);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(298, 344),    S(147, 444),    S(293, 412),    S(301, 281),    S(323, 237),    S(217, 320),    S(155, 357),    S(264, 311),    //
    S(119, 208),    S(150, 248),    S(189, 166),    S(173, 103),    S(154, 85),     S(127, 138),    S(99, 194),     S(50, 221),     //
    S(82, 173),     S(69, 176),     S(111, 102),    S(112, 80),     S(101, 77),     S(73, 109),     S(25, 159),     S(12, 187),     //
    S(58, 125),     S(48, 149),     S(84, 97),      S(80, 89),      S(59, 92),      S(42, 117),     S(-18, 160),    S(-18, 152),    //
    S(57, 99),      S(96, 106),     S(81, 137),     S(73, 123),     S(45, 119),     S(23, 125),     S(-12, 142),    S(-20, 136),    //
    S(70, 101),     S(160, 116),    S(145, 172),    S(95, 155),     S(67, 142),     S(46, 136),     S(17, 148),     S(-2, 150),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-253, -78),   S(-155, 193),   S(-101, 45),    S(93, 182),     S(30, 185),     S(-116, 189),   S(-250, 241),   S(-292, -5),    //
    S(71, 162),     S(114, 187),    S(202, 145),    S(197, 154),    S(195, 159),    S(108, 188),    S(72, 204),     S(46, 181),     //
    S(156, 158),    S(204, 152),    S(205, 209),    S(206, 193),    S(167, 227),    S(106, 244),    S(118, 188),    S(83, 181),     //
    S(225, 178),    S(234, 197),    S(230, 222),    S(217, 253),    S(218, 261),    S(172, 247),    S(165, 215),    S(160, 187),    //
    S(208, 182),    S(244, 164),    S(213, 222),    S(209, 242),    S(185, 247),    S(184, 230),    S(181, 179),    S(144, 187),    //
    S(154, 147),    S(179, 158),    S(169, 196),    S(177, 222),    S(173, 221),    S(137, 202),    S(127, 169),    S(98, 136),     //
    S(165, 151),    S(186, 148),    S(154, 157),    S(155, 181),    S(147, 179),    S(119, 136),    S(107, 156),    S(95, 72),      //
    S(100, 102),    S(145, 169),    S(161, 134),    S(177, 126),    S(158, 150),    S(126, 123),    S(111, 148),    S(73, 69),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(3, 296),      S(-11, 307),    S(-270, 347),   S(-136, 285),   S(-147, 306),   S(-159, 302),   S(-45, 302),    S(36, 274),     //
    S(125, 208),    S(67, 288),     S(101, 240),    S(47, 259),     S(64, 257),     S(88, 258),     S(125, 253),    S(101, 232),    //
    S(197, 236),    S(208, 250),    S(203, 257),    S(179, 257),    S(151, 252),    S(158, 261),    S(173, 253),    S(170, 224),    //
    S(167, 228),    S(202, 237),    S(204, 257),    S(200, 296),    S(230, 285),    S(161, 261),    S(173, 226),    S(132, 226),    //
    S(195, 187),    S(211, 219),    S(217, 242),    S(221, 271),    S(196, 286),    S(182, 263),    S(150, 240),    S(146, 198),    //
    S(221, 193),    S(255, 207),    S(266, 234),    S(211, 260),    S(207, 237),    S(202, 249),    S(208, 226),    S(149, 222),    //
    S(187, 167),    S(287, 183),    S(244, 197),    S(211, 215),    S(191, 219),    S(195, 195),    S(196, 192),    S(189, 186),    //
    S(219, 168),    S(207, 185),    S(206, 220),    S(220, 182),    S(210, 193),    S(209, 235),    S(206, 215),    S(198, 191),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(426, 379),    S(439, 398),    S(414, 412),    S(435, 373),    S(414, 380),    S(380, 390),    S(375, 408),    S(369, 411),    //
    S(301, 465),    S(376, 461),    S(453, 425),    S(399, 431),    S(384, 439),    S(364, 444),    S(297, 475),    S(280, 481),    //
    S(270, 460),    S(371, 447),    S(429, 407),    S(409, 393),    S(361, 423),    S(319, 446),    S(322, 442),    S(255, 484),    //
    S(275, 419),    S(352, 428),    S(387, 401),    S(361, 405),    S(352, 417),    S(313, 438),    S(309, 433),    S(235, 461),    //
    S(257, 352),    S(322, 369),    S(311, 389),    S(289, 386),    S(266, 410),    S(257, 422),    S(227, 417),    S(206, 414),    //
    S(249, 293),    S(311, 322),    S(297, 347),    S(282, 345),    S(270, 354),    S(244, 376),    S(242, 351),    S(207, 358),    //
    S(142, 305),    S(274, 271),    S(285, 300),    S(274, 323),    S(259, 329),    S(252, 332),    S(234, 319),    S(210, 328),    //
    S(207, 290),    S(230, 311),    S(286, 301),    S(298, 298),    S(287, 306),    S(267, 321),    S(256, 317),    S(237, 326),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(550, 643),    S(601, 593),    S(523, 719),    S(493, 785),    S(476, 772),    S(511, 713),    S(533, 636),    S(489, 670),    //
    S(550, 697),    S(514, 793),    S(503, 827),    S(359, 923),    S(366, 911),    S(429, 859),    S(475, 731),    S(484, 714),    //
    S(513, 764),    S(560, 784),    S(492, 890),    S(443, 906),    S(416, 903),    S(467, 821),    S(504, 740),    S(525, 657),    //
    S(553, 683),    S(556, 772),    S(492, 845),    S(484, 873),    S(477, 870),    S(487, 777),    S(530, 710),    S(517, 676),    //
    S(567, 651),    S(554, 724),    S(525, 780),    S(495, 822),    S(493, 824),    S(497, 768),    S(514, 691),    S(541, 612),    //
    S(547, 595),    S(581, 645),    S(573, 713),    S(542, 707),    S(539, 694),    S(533, 722),    S(549, 638),    S(527, 611),    //
    S(529, 481),    S(566, 495),    S(570, 563),    S(581, 601),    S(568, 627),    S(562, 592),    S(525, 632),    S(537, 606),    //
    S(511, 497),    S(543, 343),    S(567, 364),    S(581, 474),    S(576, 551),    S(575, 497),    S(563, 522),    S(532, 550),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-58, -323),   S(289, 177),    S(160, 136),    S(34, -31),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(231, -65),    S(347, 162),    S(202, 155),    S(122, 4),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(227, 27),     S(382, 105),    S(268, 108),    S(164, 8),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(83, -5),      S(332, 43),     S(180, 66),     S(117, 30),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-23, -26),    S(229, 9),      S(157, 18),     S(39, 28),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-23, -18),    S(210, -1),     S(113, 10),     S(59, -6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-3, -20),     S(81, 36),      S(-3, 39),      S(-60, 16),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-115, -72),   S(-17, -25),    S(-117, -18),   S(-120, -87),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, -17), S(-2, 18), S(-5, 20), S(2, 6), S(2, 2), S(-5, 20), S(0, 19), }},
  {{ S(6, -6), S(-21, 27), S(-17, 21), S(-8, 3), S(-8, 3), S(-17, 25), S(-20, 35), }},
  {{ S(-13, -0), S(-24, 25), S(-22, 7), S(-18, -7), S(-18, -6), S(-21, 7), S(-22, 22), }},
  {{ S(2, -16), S(-16, -16), S(-7, -19), S(-2, -23), S(-3, -22), S(-8, -17), S(-11, -17), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(1, 26), S(-21, 33), S(-18, 22), S(-11, 10), S(-10, -7),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-7, 4), S(-17, 13), S(-16, 11), S(-15, 11), S(-15, 6), S(-14, 2), S(-17, 11), }},
  {{ S(-6, 3), S(-14, 9), S(-20, 14), S(-17, 12), S(-17, 8), S(-19, 11), S(-13, 8), }},
  {{ S(-12, 7), S(-17, 5), S(-18, 12), S(-14, 11), S(-13, 10), S(-17, 10), S(-16, 6), }},
  {{ S(-13, 9), S(-18, 11), S(-19, 11), S(-14, 15), S(-12, 15), S(-18, 11), S(-19, 11), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
	1181, 554, -25, 55
);

inline VParam WINNABLE_PAWNS = V(-15);
inline VParam WINNABLE_SYM = V(137);
inline VParam WINNABLE_ASYM = V(124);
inline VParam WINNABLE_PAWN_ENDGAME = V(225);
inline VParam WINNABLE_BIAS = V(-655);

// Epoch duration: 7.52381s
// clang-format on
}  // namespace Clockwork
