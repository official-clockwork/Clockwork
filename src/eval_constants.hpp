#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(140, 194);
inline const PParam KNIGHT_MAT = S(591, 590);
inline const PParam BISHOP_MAT = S(635, 620);
inline const PParam ROOK_MAT   = S(495, 643);
inline const PParam QUEEN_MAT  = S(1166, 1021);
inline const PParam TEMPO_VAL  = S(59, 20);

inline const PParam BISHOP_PAIR_VAL   = S(54, 190);
inline const PParam ROOK_OPEN_VAL     = S(106, -12);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 23);

inline const PParam DOUBLED_PAWN_VAL = S(-6, -67);
inline const PParam ISOLATED_PAWN_VAL = S(-19, -20);

inline const PParam POTENTIAL_CHECKER_VAL = S(-51, -21);
inline const PParam OUTPOST_KNIGHT_VAL    = S(36, 63);
inline const PParam OUTPOST_BISHOP_VAL    = S(50, 38);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(43, 5);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(56, -20);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(28, 50);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(70, -41);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(17, -0), S(40, 24), S(58, 49), S(136, 148), S(359, 242), S(442, 735),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(55, 32), S(46, 24), S(55, 51), S(121, 140), S(423, 73),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-79, -112), S(-74, -89), S(-49, 18), S(12, 120), S(104, 256), S(279, 359),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(34, -41), S(32, 0), S(21, 39), S(2, 112), S(57, 212), S(204, 277),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(23, -46), S(4, 6), S(0, -22), S(-5, -50), S(-16, -113), S(-221, -216),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-6, 152), S(-16, 116), S(-9, 45), S(-3, 9), S(2, 7), S(34, 6), S(5, 3),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-329, -35), S(-49, 27), S(-27, 60), S(13, 98), S(34, 116), S(55, 118), S(35, 115),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(66, 3), S(137, 172), S(173, 258), S(201, 293), S(241, 310), S(266, 343), S(298, 341), S(329, 355), S(381, 272),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(85, -2), S(129, 168), S(184, 228), S(211, 272), S(236, 302), S(253, 325), S(260, 344), S(275, 353), S(281, 371), S(305, 355), S(317, 354), S(365, 304), S(365, 315), S(391, 276),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(317, 237), S(237, 410), S(263, 434), S(280, 443), S(291, 455), S(298, 466), S(304, 476), S(314, 477), S(320, 487), S(332, 487), S(344, 488), S(353, 490), S(358, 489), S(374, 465), S(478, 344),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(494, 521), S(649, 495), S(674, 604), S(692, 716), S(708, 763), S(716, 806), S(722, 837), S(730, 847), S(733, 871), S(737, 883), S(743, 888), S(748, 895), S(757, 890), S(760, 892), S(765, 889), S(765, 889), S(767, 886), S(775, 876), S(783, 865), S(789, 864), S(801, 836), S(826, 793), S(820, 799), S(820, 740), S(807, 730), S(799, 706), S(735, 768), S(756, 666),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(481, -173), S(107, -108), S(29, -27), S(16, 6), S(-3, 6), S(-21, 3), S(-23, 7), S(-38, 16), S(-27, -24),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(12, -8), S(15, -16), S(13, -12), S(6, -8), S(2, 179),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -0), S(7, -11), S(6, -9), S(5, -5), S(6, 429),
};

inline const PParam PAWN_THREAT_KNIGHT = S(216, 87);
inline const PParam PAWN_THREAT_BISHOP = S(193, 138);
inline const PParam PAWN_THREAT_ROOK   = S(196, 116);
inline const PParam PAWN_THREAT_QUEEN  = S(175, -37);

inline const PParam KNIGHT_THREAT_BISHOP = S(112, 95);
inline const PParam KNIGHT_THREAT_ROOK   = S(232, 57);
inline const PParam KNIGHT_THREAT_QUEEN  = S(153, -6);

inline const PParam BISHOP_THREAT_KNIGHT = S(107, 55);
inline const PParam BISHOP_THREAT_ROOK   = S(216, 117);
inline const PParam BISHOP_THREAT_QUEEN  = S(179, 93);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(7, -31), S(-3, -10), S(-6, -18), S(-9, -28), S(-13, -37), S(-18, -42), S(-20, -54), S(-27, -51), S(-38, -51),
};

inline const PParam ROOK_LINEUP = S(11, 72);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(313, 328),    S(167, 420),    S(300, 401),    S(315, 264),    S(333, 225),    S(227, 306),    S(166, 341),    S(266, 306),    //
    S(117, 207),    S(148, 241),    S(190, 158),    S(175, 96),     S(153, 82),     S(125, 133),    S(97, 189),     S(50, 216),     //
    S(82, 169),     S(68, 169),     S(110, 99),     S(111, 79),     S(99, 76),      S(71, 107),     S(24, 154),     S(13, 183),     //
    S(55, 126),     S(45, 146),     S(81, 99),      S(79, 89),      S(57, 91),      S(41, 115),     S(-19, 156),    S(-18, 149),    //
    S(54, 101),     S(94, 105),     S(79, 137),     S(73, 122),     S(44, 117),     S(22, 123),     S(-12, 139),    S(-20, 134),    //
    S(68, 101),     S(160, 113),    S(144, 170),    S(96, 150),     S(66, 139),     S(46, 132),     S(16, 144),     S(-2, 148),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-266, -64),   S(-145, 183),   S(-143, 91),    S(92, 180),     S(38, 173),     S(-122, 199),   S(-231, 210),   S(-303, 4),     //
    S(67, 165),     S(118, 178),    S(201, 144),    S(188, 167),    S(180, 180),    S(109, 187),    S(72, 204),     S(50, 173),     //
    S(162, 147),    S(194, 168),    S(207, 204),    S(200, 201),    S(169, 223),    S(108, 239),    S(119, 187),    S(86, 176),     //
    S(228, 172),    S(236, 194),    S(233, 214),    S(220, 247),    S(221, 253),    S(176, 240),    S(168, 209),    S(164, 178),    //
    S(211, 177),    S(244, 162),    S(216, 216),    S(213, 234),    S(189, 240),    S(187, 225),    S(185, 173),    S(148, 179),    //
    S(155, 146),    S(182, 153),    S(172, 191),    S(181, 215),    S(177, 214),    S(141, 195),    S(130, 164),    S(101, 133),    //
    S(167, 149),    S(189, 143),    S(156, 154),    S(159, 174),    S(150, 174),    S(122, 132),    S(107, 158),    S(96, 75),      //
    S(101, 103),    S(147, 163),    S(164, 130),    S(181, 123),    S(162, 144),    S(128, 119),    S(113, 143),    S(73, 70),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-3, 287),     S(-12, 297),    S(-285, 354),   S(-155, 292),   S(-164, 313),   S(-186, 320),   S(-58, 305),    S(22, 281),     //
    S(123, 196),    S(65, 279),     S(93, 238),     S(32, 268),     S(47, 267),     S(78, 258),     S(119, 249),    S(101, 220),    //
    S(194, 226),    S(203, 245),    S(200, 248),    S(176, 247),    S(146, 245),    S(154, 253),    S(172, 240),    S(168, 213),    //
    S(164, 217),    S(200, 225),    S(198, 251),    S(198, 282),    S(230, 269),    S(158, 251),    S(170, 215),    S(131, 213),    //
    S(192, 176),    S(209, 206),    S(214, 231),    S(221, 255),    S(196, 271),    S(180, 250),    S(147, 229),    S(142, 191),    //
    S(218, 184),    S(255, 193),    S(265, 221),    S(210, 246),    S(206, 224),    S(200, 236),    S(208, 212),    S(146, 214),    //
    S(186, 155),    S(286, 172),    S(243, 185),    S(209, 205),    S(189, 208),    S(194, 182),    S(193, 183),    S(187, 176),    //
    S(217, 158),    S(205, 177),    S(204, 208),    S(220, 167),    S(209, 181),    S(208, 222),    S(205, 202),    S(195, 180),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(421, 375),    S(445, 384),    S(417, 399),    S(434, 367),    S(412, 374),    S(377, 387),    S(377, 399),    S(365, 407),    //
    S(303, 453),    S(383, 444),    S(462, 405),    S(407, 414),    S(385, 429),    S(373, 424),    S(300, 461),    S(279, 472),    //
    S(270, 451),    S(373, 434),    S(431, 395),    S(414, 379),    S(365, 408),    S(324, 430),    S(325, 430),    S(257, 471),    //
    S(277, 410),    S(355, 415),    S(390, 390),    S(368, 389),    S(358, 401),    S(321, 419),    S(316, 416),    S(238, 447),    //
    S(259, 344),    S(327, 354),    S(315, 376),    S(295, 373),    S(274, 394),    S(263, 408),    S(230, 407),    S(210, 403),    //
    S(250, 288),    S(313, 312),    S(301, 336),    S(286, 334),    S(272, 344),    S(248, 365),    S(242, 345),    S(205, 355),    //
    S(145, 295),    S(276, 264),    S(286, 293),    S(277, 314),    S(263, 319),    S(252, 327),    S(237, 307),    S(210, 321),    //
    S(206, 287),    S(229, 309),    S(286, 296),    S(299, 293),    S(288, 302),    S(268, 316),    S(257, 312),    S(237, 321),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(567, 609),    S(619, 561),    S(544, 685),    S(509, 759),    S(498, 736),    S(522, 694),    S(548, 610),    S(493, 661),    //
    S(563, 674),    S(530, 767),    S(512, 813),    S(375, 897),    S(377, 890),    S(444, 834),    S(485, 716),    S(499, 688),    //
    S(528, 738),    S(571, 766),    S(515, 851),    S(455, 887),    S(428, 882),    S(477, 804),    S(520, 712),    S(538, 635),    //
    S(570, 650),    S(575, 737),    S(510, 814),    S(501, 845),    S(488, 854),    S(500, 757),    S(551, 673),    S(531, 653),    //
    S(584, 621),    S(574, 687),    S(542, 750),    S(513, 791),    S(510, 796),    S(509, 750),    S(531, 662),    S(557, 588),    //
    S(563, 566),    S(598, 617),    S(591, 681),    S(558, 680),    S(555, 668),    S(550, 691),    S(564, 612),    S(543, 584),    //
    S(543, 459),    S(583, 466),    S(589, 528),    S(599, 572),    S(583, 604),    S(577, 569),    S(542, 601),    S(552, 582),    //
    S(527, 470),    S(557, 327),    S(582, 345),    S(596, 456),    S(592, 528),    S(591, 475),    S(579, 498),    S(545, 533),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-67, -360),   S(261, 213),    S(164, 128),    S(35, -43),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(238, -87),    S(334, 188),    S(178, 181),    S(141, -24),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(225, 27),     S(379, 100),    S(276, 93),     S(185, -22),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(72, 2),       S(323, 43),     S(169, 63),     S(107, 25),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-31, -16),    S(230, 7),      S(148, 20),     S(15, 38),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-21, -13),    S(215, -5),     S(109, 12),     S(60, -6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-1, -16),     S(84, 32),      S(-5, 40),      S(-60, 16),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-115, -66),   S(-16, -22),    S(-119, -13),   S(-120, -81),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(15, -17), S(-3, 18), S(-7, 21), S(-0, 8), S(-0, 4), S(-7, 21), S(-1, 21), }},
  {{ S(4, -3), S(-23, 30), S(-19, 24), S(-11, 7), S(-10, 7), S(-19, 29), S(-22, 38), }},
  {{ S(-14, 4), S(-26, 31), S(-24, 12), S(-20, -1), S(-20, -0), S(-23, 12), S(-24, 27), }},
  {{ S(1, -12), S(-17, -12), S(-9, -15), S(-4, -19), S(-5, -17), S(-9, -13), S(-13, -13), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(5, 26), S(-16, 31), S(-14, 20), S(-6, 8), S(-7, -8),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-3, 4), S(-13, 12), S(-12, 10), S(-11, 10), S(-11, 5), S(-10, 2), S(-13, 9), }},
  {{ S(-2, 2), S(-10, 8), S(-16, 13), S(-13, 10), S(-13, 6), S(-15, 10), S(-9, 7), }},
  {{ S(-8, 5), S(-13, 3), S(-14, 9), S(-10, 9), S(-9, 7), S(-13, 8), S(-12, 4), }},
  {{ S(-9, 7), S(-14, 9), S(-15, 8), S(-10, 12), S(-8, 12), S(-14, 8), S(-16, 8), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1192, 541, -31, 51
);

inline VParam WINNABLE_PAWNS = V(47);
inline VParam WINNABLE_BIAS = V(-451);

// Epoch duration: 7.46582s
// clang-format on
}  // namespace Clockwork
