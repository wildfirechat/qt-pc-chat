/*
 * Copyright (c) 2023 Alex Spataru <https://github.com/alex-spataru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <QImage>
#include <QObject>
#include <QString>

namespace qrcode {

enum class ErrorCorrection {
    LOW = 0,  // The QR Code can tolerate about  7% erroneous codewords
    MEDIUM,   // The QR Code can tolerate about 15% erroneous codewords
    QUARTILE, // The QR Code can tolerate about 25% erroneous codewords
    HIGH,     // The QR Code can tolerate about 30% erroneous codewords
};

/**
 * @brief Generates a QR code from the given data and error correction level.
 * @param data The QString containing the data to encode in the QR code.
 * @param size The desired width/height of the generated image (default: 500).
 * @param borderSize The desired border width of the generated image (default:
 * 1).
 * @param errorCorrection The desired error correction level (default:
 * qrcodegen::QrCode::Ecc::MEDIUM).
 *
 * @return QImage containing the generated QR code.
 */
QImage generateQr(
    const QString &data,
    const quint16 size = 1000,
    const quint16 borderSize = 1,
    ErrorCorrection errorCorrection = ErrorCorrection::MEDIUM
);

/**
 * @brief Generates a QR code from the given data and error correction level.
 * @param data The QString containing the data to encode in the QR code.
 * @param borderSize The desired border width of the generated image (default:
 * 1).
 * @param errorCorrection The desired error correction level (default:
 * qrcodegen::QrCode::Ecc::MEDIUM).
 *
 * @return QString string containing the generated QR code in SVG format.
 */
QString generateSvgQr(
    const QString &data,
    const quint16 borderSize = 1,
    ErrorCorrection errorCorrection = ErrorCorrection::MEDIUM
);
}
