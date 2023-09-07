import { APIGatewayProxyHandler, APIGatewayProxyResult } from 'aws-lambda';
import { S3Client, PutObjectCommand } from '@aws-sdk/client-s3';

export const handler: APIGatewayProxyHandler = async (event): Promise<APIGatewayProxyResult> => {
  if (!event.body) {
    return {
      statusCode: 400,
      body: JSON.stringify({ message: 'Invalid request' }),
    };
  }

  const contentDispositionHeader = event.headers['Content-Disposition'];
  const filenameMatch = /filename="(.*)"/.exec(contentDispositionHeader!);
  const filename = filenameMatch && filenameMatch[1] ? filenameMatch[1] : 'image.jpg';

  try {
    const decodedImage = Buffer.from(event.body, 'base64');
    const putObjectRequest = {
      Bucket: process.env.BUCKET!,
      Key: filename,
      Body: decodedImage,
    };

    await new S3Client().send(new PutObjectCommand(putObjectRequest));

    return {
      statusCode: 200,
      body: JSON.stringify({ message: 'Image saved to S3 successfully' }),
    };
  } catch (error) {
    console.error('Error:', error);

    return {
      statusCode: 500,
      body: JSON.stringify({ message: 'An error occurred while saving the image to S3' }),
    };
  }
};
